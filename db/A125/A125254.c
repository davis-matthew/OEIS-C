/*
 * A125254  Smallest prime divisor of 4n-1 that is of the form 4k-1.
 * https://oeis.org/A125254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime divisor of 4*n-1 that is ≡3 (mod 4)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return 0;                     /* undefined for n<1, harmless */

    /* m = 4*n - 1, kept in seq_t so arbitrary‑precision is handled */
    seq_t m = (seq_t)n * (seq_t)4 - (seq_t)1;

    /* trial divide by odd candidates */
    for (seq_t p = (seq_t)3; p * p <= m; p += (seq_t)2)
    {
        if (m % p == (seq_t)0)
        {
            /* if the divisor itself is ≡3 (mod 4) we are done */
            if (p % (seq_t)4 == (seq_t)3)
                return p;

            /* otherwise divide all copies of this prime out of m */
            while (m % p == (seq_t)0)
                m /= p;
        }
    }

    /* if any factor remains, it must be a prime ≡3 (mod 4) */
    return m > (seq_t)1 ? m : (seq_t)0;
}
