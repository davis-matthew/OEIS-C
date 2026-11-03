/*
 * A202869  Symmetric matrix based on the lower Wythoff sequence, A000201, by
 *          antidiagonals.
 * https://oeis.org/A202869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{k=1}^{min(i,j)} s[i‑k+1] · s[j‑k+1] where s[m] = (m +
 *            ⌊m·√5⌋)/2 is the lower Wythoff sequence A000201 and (i,j) are
 *            the antidiagonal coordinates of the n‑th entry in T′·T
 * Range:     u8 n <= 6, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Cache for the lower Wythoff sequence, 1‑based. */
static seq_t *low_cache   = NULL;   /* pointer to cached values */
static size_t low_cache_cap = 0;    /* current capacity */
static size_t low_cache_len = 0;    /* largest index that is cached */

/* Ensure that low_cache contains values up to index n (inclusive). */
static void ensure_low_cache(size_t n)
{
    if (n <= low_cache_len)
        return;

    size_t new_cap = low_cache_cap ? low_cache_cap : 16;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, low_cache, new_cap);
    low_cache = tmp;
    low_cache_cap = new_cap;

    for (size_t k = low_cache_len + 1; k <= n; ++k) {
        seq_t kseq = (seq_t)k;
        __uint128_t tmp_val = (__uint128_t)k * k * 5;
        seq_t sq = floor_sqrt((seq_t)tmp_val);       /* ⌊k·√5⌋ */
        low_cache[k] = (kseq + sq) / (seq_t)2;       /* s[k] */
    }
    low_cache_len = n;
}

/* Compute the antidiagonal (row i, column j) corresponding to the n‑th entry. */
static void antidiag_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t s = 1;
    while (1) {
        __uint128_t tot = (__uint128_t)s * (s + 1) / 2;
        if (tot >= n)
            break;
        ++s;
    }
    __uint128_t prev = (__uint128_t)(s - 1) * s / 2;
    idx_t offset = n - (idx_t)prev;      /* 1‑based position on this diagonal */
    *i = offset;
    *j = s - offset + 1;                 /* j decreases as i increases */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* behaviour below offset is irrelevant */

    idx_t i, j;
    antidiag_coords(n, &i, &j);

    idx_t max_idx = i > j ? i : j;
    ensure_low_cache((size_t)max_idx);

    idx_t k_max = i < j ? i : j;
    seq_t sum = 0;

    for (idx_t k = 1; k <= k_max; ++k) {
        seq_t a1 = low_cache[(size_t)(i - k + 1)];
        seq_t a2 = low_cache[(size_t)(j - k + 1)];
        sum += a1 * a2;
    }
    return sum;
}
