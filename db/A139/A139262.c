/*
 * A139262  Total number of two-element anti-chains over all ordered trees on
 *          n edges.
 * https://oeis.org/A139262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n + 1) * binom(2n, n) - 4^n) / 2 (OEIS A139262)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The types seq_t (arbitrary precision integer) and idx_t (uint64_t) are
   supplied by the build system.  The arithmetic operators work as for C
   integers. */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic.  Use a local variable so
       that the code stays clear and the compiler can optimise. */
    seq_t sn = (seq_t)n;
    seq_t sn_plus_one = sn + 1;

    /* Compute the central binomial coefficient binom(2n, n) multiplicatively
       to avoid large factorials.  For each i = 1..n
       res = res * (n + i) / i    (the division is exact). */
    seq_t bin = 1;
    for (idx_t i = 1; i <= n; ++i) {
        bin = bin * (sn + (seq_t)i) / (seq_t)i;
    }

    /* Compute 4^n by repeated squaring.  This is O(log n) but even a
       linear loop over n works well for n <= 10000.  We choose the linear
       version for simplicity. */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow4 = pow4 * 4;
    }

    /* 2*a(n) = (n+1)*binom(2n,n) - 4^n  ->  a(n) = ((n+1)*bin - pow4)/2. */
    seq_t numerator = sn_plus_one * bin - pow4;
    return numerator / 2;
}
