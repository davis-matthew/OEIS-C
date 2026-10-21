/*
 * A099172  Array T(m, n) read by antidiagonals: number of binary strings with
 *          m 1's and n 0's without zigzags.
 * https://oeis.org/A099172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(i,j) where T(m,n) = Σ_{k=0}^{min(m+⌊m/2⌋, n+⌊n/2⌋)} 
 *   C(m−k+2⌊k/3⌋, ⌊k/3⌋) · C(n−k+2⌊k/3⌋, ⌊k/3⌋)
 * The pair (i,j) is obtained from the index n by reading the array T
 * by antidiagonals: if the cumulative number of terms up to diagonal d is
 * (d+1)(d+2)/2, then n lies on diagonal d with offset i in [0,d] and j=d−i.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine (i,j) from the index n. */
    idx_t d = 0;
    idx_t rem = n;

    /* Subtract lengths of complete diagonals until the remaining n falls
       inside the current diagonal.  Length of diagonal d is d+1. */
    while (rem >= (d + 1)) {
        rem -= (d + 1);
        d++;
    }
    idx_t i = rem;          /* number of 1's */
    idx_t j = d - rem;      /* number of 0's */

    /* Compute T(i,j) by the given summation. */
    seq_t result = 0;
    idx_t max_k = (i + (i / 2)) < (j + (j / 2)) ? (i + (i / 2)) : (j + (j / 2));

    for (idx_t k = 0; k <= max_k; k++) {
        idx_t r = k / 3;                       /* ⌊k/3⌋ */
        /* Compute the top argument of the binomial coefficients.  Use signed
           arithmetic to avoid unsigned underflow when k > i or k > j. */
        int64_t a_val = (int64_t)i - (int64_t)k + 2 * (int64_t)r;
        int64_t c_val = (int64_t)j - (int64_t)k + 2 * (int64_t)r;

        if (a_val < (int64_t)r || c_val < (int64_t)r)
            continue;   /* binomial would be 0; skip */

        seq_t t1 = binom((idx_t)a_val, r);
        seq_t t2 = binom((idx_t)c_val, r);
        result += t1 * t2;
    }
    return result;
}
