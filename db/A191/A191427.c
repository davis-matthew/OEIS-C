/*
 * A191427  Dispersion of ([n*r+3/2]), where r=(golden ratio)=(1+sqrt(5))/2
 *          and [ ]=floor, by antidiagonals.
 * https://oeis.org/A191427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = dispersion of ⌊n·φ+3/2⌋ by antidiagonals
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

/* Compute s(n) = ⌊n·φ+3/2⌋, where φ = (1+√5)/2.           */
/* Uses only integer arithmetic and floor_sqrt.           */
static seq_t s_big(seq_t n)
{
    /* floor(n·√5) = floor(√(5·n²)) */
    seq_t prod = 5 * n * n;
    seq_t sq   = floor_sqrt(prod);
    /* (sq + n + 3) / 2  (integer division) */
    return (sq + n + (seq_t)3) >> 1;
}

/* Find the smallest d such that d(d+1)/2 ≥ n (1‑based diagonal). */
static idx_t diagonal_from_n(idx_t n)
{
    idx_t lo = 1, hi = n;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* ----------  storage for the complement sequence t(i)  ---------- */

static idx_t      next_s_index = 1;   /* next n for s(n) */
static seq_t      next_m       = 1;   /* next integer to test */
static size_t     t_cap        = 0;
static size_t     t_len        = 0;
static seq_t     *t_arr        = NULL;   /* array of t(i) values */

/* Ensure t_arr contains at least 'need' entries. */
static void ensure_t(size_t need)
{
    if (t_len >= need)
        return;

    /* grow array */
    size_t newcap = t_cap ? t_cap * 2 : 4;
    if (newcap < need)
        newcap = need;
    seq_t *newptr = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newptr)
        abort();
    if (t_arr) {
        memcpy(newptr, t_arr, t_len * sizeof(seq_t));
        free(t_arr);
    }
    t_arr = newptr;
    t_cap = newcap;

    /* generate t values until we have 'need' of them */
    while (t_len < need)
    {
        seq_t sval = s_big(next_s_index);
        while (next_m < sval) {
            t_arr[t_len++] = next_m;
            next_m++;
        }
        /* skip the integer that belongs to s */
        next_m++;
        next_s_index++;
    }
}

/* ----------  Main sequence function  ---------- */

seq_t a(idx_t n)
{
    /* locate the diagonal (1‑based) */
    idx_t d = diagonal_from_n(n);

    /* number of entries before this diagonal */
    __uint128_t prev128 = (__uint128_t)d * (d - 1) / 2;
    idx_t prev = (idx_t)prev128;

    /* position on this diagonal (0‑based) */
    idx_t offset = n - prev - 1;
    idx_t i = offset + 1;               /* index into t (1‑based) */
    idx_t j = d - i + 1;                /* number of s‑applications = j‑1 */

    /* get t(i) */
    ensure_t(i);
    seq_t val = t_arr[(size_t)(i - 1)];

    /* apply s j‑1 times */
    for (idx_t step = 1; step < j; ++step)
        val = s_big(val);

    return val;
}
