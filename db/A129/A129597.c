/*
 * A129597  Central diagonal of array A129595.
 * https://oeis.org/A129597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, for n > 1 let g be the largest prime factor of n then
 *            a(n) = 2*n^2 / g (i.e. a(n) = 2*n iff n is prime)
 * Range:     u8 n <= 11, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    idx_t m = n;          /* mutable copy for factorisation */
    idx_t g = 0;          /* largest prime factor found */

    /* handle factor 2 */
    if ((m & 1) == 0) {
        g = 2;
        while ((m & 1) == 0) m >>= 1;
    }

    /* trial division by odd numbers up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            g = p;
            while (m % p == 0) m /= p;
        }
    }

    /* if remaining m > 1 it is a prime factor larger than all seen */
    if (m > 1) g = m;

    /* compute result: 2*n*n / g */
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;  /* n^2 */
    val *= (seq_t)2;      /* 2*n^2 */
    return val / (seq_t)g;
}
