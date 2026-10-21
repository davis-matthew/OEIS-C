/*
 * A095196  Duplicate of A074828.
 * https://oeis.org/A095196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   * a(1) = 1, * a(2) = 4, * for n > 2: * if n is even, a(n) = 2, *
 *            if n is odd, * if n is prime, a(n) = 2*n, * else a(n) = smallest
 *            prime factor of n. * * This implements the sequence A095196
 *            (duplicate of A074828)
 * Range:     i8 n <= 31, i16 n <= 31, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two special cases */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)4;

    /* For all n > 2 */
    if ((n & (idx_t)1) == 0)   /* even n */
        return (seq_t)2;

    /* n is odd and >= 3 */
    /* Try to find a small divisor.  If none found, n is prime. */
    for (idx_t p = (idx_t)3; p <= n / p; p += (idx_t)2) {
        if ((n % p) == 0)
            return (seq_t)p;  /* smallest prime factor */
    }

    /* n is odd prime */
    return (seq_t)n * (seq_t)2;
}
