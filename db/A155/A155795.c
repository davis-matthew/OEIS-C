/*
 * A155795  Triangle read by rows: t(n,k)=n!/(n - k*(n - k)).
 * https://oeis.org/A155795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   t(n,k) = n! / ((n - k*(n - k))!) for 0 ≤ k ≤ n; If n - k*(n - k)
 *            < 0, t(n,k) = 0. a(n) is the triangle read by rows
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* integer square‑root of an unsigned 64–bit integer */
static uint64_t u64_isqrt(uint64_t v)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == v) return mid;
        if (sq < v) lo = mid + 1;
        else        hi = mid - 1;
    }
    return hi;
}

/* factorial cache ----------------------------------------------------- */
static seq_t   *fac = NULL;   /* factorials: fac[n] = n! */
static uint64_t fac_cap = 0;  /* capacity of fac array */
static uint64_t fac_len = 0;  /* next free index in fac */

/* ensure fac[n] is computed */
static void ensure_fac(uint64_t n)
{
    if (n < fac_len) return;

    /* grow cache to at least n+1 */
    size_t new_cap = (fac_cap == 0) ? 16 : fac_cap;
    while (new_cap <= n) new_cap <<= 1;
    fac = seq_realloc(fac, fac_cap, new_cap);
    fac_cap = new_cap;

    if (fac_len == 0) {
        fac[0] = (seq_t)1;
        fac_len = 1;
    }

    for (uint64_t i = fac_len; i <= n; ++i) {
        fac[i] = fac[i-1] * (seq_t)i;
    }
    fac_len = n + 1;
}

/* main sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    uint64_t idx = (uint64_t)n;            /* global index */
    uint64_t m   = idx;

    /* find row r such that r(r+1)/2 ≤ m < (r+1)(r+2)/2 */
    uint64_t s = u64_isqrt(8*m + 1);
    uint64_t r = (s - 1) / 2;

    uint64_t base = r * (r + 1) / 2;
    uint64_t k    = m - base;             /* column within the row */

    /* compute d = r - k*(r-k) */
    __int128 prod = (__int128)k * (r - k);
    __int128 d128 = (__int128)r - prod;
    if (d128 < 0) return (seq_t)0;
    uint64_t d = (uint64_t)d128;

    /* compute factorials */
    ensure_fac(r);
    ensure_fac(d);

    seq_t num   = fac[r];
    seq_t denom = fac[d];

    return num / denom;   /* exact division */
}
