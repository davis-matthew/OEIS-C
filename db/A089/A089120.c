/*
 * A089120  Smallest prime factor of n^2 + 1.
 * https://oeis.org/A089120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime factor of n^2 + 1 (for n >= 1, offset 1;
 *            if n is odd, a(n)=2; otherwise return the least prime p such
 *            that p | (n^2 + 1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term */
    if (n == 0) return 0;          /* undefined for offset, but safe */
    /* n odd → n^2+1 is even, and 2 is the smallest prime factor */
    if (n & 1) return 2;

    /* m = n^2 + 1 (fits in seq_t for all supported builds) */
    seq_t m = (seq_t)n * (seq_t)n + (seq_t)1;

    /* Search for the smallest odd prime divisor of m.
       We start at 3 and test odd candidates only. */
    seq_t p = 3;
    /* Continue while p*p <= m */
    while (p * p <= m) {
        /* if m % p == 0, p is the smallest prime factor */
        if ((m % p) == 0)
            return p;
        p += 2;  /* next odd candidate */
    }
    /* No divisor found: m itself is prime */
    return m;
}
