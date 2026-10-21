/*
 * A056832  All a(n) = 1 or 2; a(1) = 1; get next 2^k terms by repeating first
 *          2^k terms and changing last element so sum of first 2^(k+1) terms
 *          is odd.
 * https://oeis.org/A056832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, mult
 * Formula:   a(1)=1; for n>1 let m be the largest power of 2 < n. If n==m
 *            then a(n)=1 + (log₂ n mod 2) (i.e. 1 if log₂ n even, 2 if odd).
 *            Otherwise a(n)=a(n-m). This reproduces the period‑doubling
 *            (Toeplitz) sequence with values 1 and 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;               /* a(1)=1 */

    /* Reduce n to a power of 2 by repeatedly subtracting the highest
       power of 2 that is less than n. */
    while ((n & (n - 1)) != 0) {        /* n not a power of 2 */
        /* position of the most significant bit of n */
        int msb = 63 - __builtin_clzll(n);   /* works for uint64_t */
        idx_t high = (idx_t)1 << msb;   /* 2^msb */
        n -= high;                     /* n' = n - 2^msb */
    }

    /* Now n is a power of 2.  Compute log₂(n) parity. */
    int msb = 63 - __builtin_clzll(n);
    return (msb & 1) ? 2 : 1;           /* parity of exponent */
}
