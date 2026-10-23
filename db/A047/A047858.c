/*
 * A047858  T(n, k) = 2^(k-1)*(k + 2*n) - n + 1, array read by descending
 *          antidiagonals.
 * https://oeis.org/A047858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047858.  Formula: a(n) = T(row, col) where
   T(n, k) = 2^(k-1)*(k + 2*n) - n + 1,
   and (row, col) correspond to the anti‑diagonal enumeration of the
   infinite array T. */

/* ---- helpers ----------------------------------------------------- */
#include <stdint.h>
#include "../utils/pow2.h"          /* pow2(idx_t exp) -> seq_t */

/* integer square root for a 128‑bit unsigned integer */
static inline uint64_t isqrt_u128(__uint128_t x)
{
    uint64_t hi = 1;
    /* grow hi until hi*hi > x */
    while ( ( (__uint128_t)hi * hi ) <= x )
        hi <<= 1;
    uint64_t lo = hi >> 1;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ( ( (__uint128_t)mid * mid ) <= x )
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ---- main function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find the anti‑diagonal number d such that
       d*(d+1)/2 <= n < (d+1)*(d+2)/2  */
    __uint128_t val = (__uint128_t)8 * n + 1;          /* 8*n+1 */
    uint64_t   s   = isqrt_u128(val);                  /* floor(sqrt(8*n+1)) */
    uint64_t   d   = (s - 1) / 2;                      /* d = floor((s-1)/2) */

    /* Position inside the diagonal */
    __uint128_t tri = (__uint128_t)d * (d + 1) / 2;     /* d*(d+1)/2 */
    uint64_t   pos = (uint64_t)(n - (uint64_t)tri);    /* 0 <= pos <= d */

    uint64_t   row = pos;          /* n in the T(n, k) formula */
    uint64_t   col = d - pos;      /* k in the T(n, k) formula */

    /* Compute T(row, col) */
    seq_t term;
    if (col == 0) {
        term = (seq_t)row;                       /* 2^(−1)*2*row = row */
    } else {
        seq_t mult = pow2(col - 1);              /* 2^(col-1) */
        seq_t tmp  = (seq_t)col + ((seq_t)row << 1);
        term = mult * tmp;
    }

    return term - (seq_t)row + 1;
}
