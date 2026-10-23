/*
 * A014963  Exponential of Mangoldt function M(n): a(n) = 1 unless n is a
 *          prime or prime power, in which case a(n) = that prime.
 * https://oeis.org/A014963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 unless n is a prime or prime power, in which case a(n)
 *            = that prime
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;               /* a(1) = 1 */

    /* Find the smallest prime divisor of n */
    idx_t spf = 0;
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            spf = i;
            break;
        }
    }

    if (spf == 0)              /* n is prime */
        return (seq_t)n;

    /* Check if n is a power of spf */
    idx_t t = n;
    while (t % spf == 0)
        t /= spf;

    if (t == 1)                /* n = spf^k */
        return (seq_t)spf;
    else
        return (seq_t)1;       /* n is not a prime power */
}
