/*
 * A143272  a(n) = d(n)*T(n), where d(n) is the number of divisors of n
 *          (A000005) and T(n)=n(n+1)/2 are the triangular numbers (A000217).
 * https://oeis.org/A143272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) * T(n) where d(n) is the number of divisors of n *
 *            and T(n) = n*(n+1)/2 is the nth triangular number. * The
 *            computation is done by factoring n on the fly
 * Range:     u8 n <= 11, u16 n <= 107, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* may help for types, but idx_t and seq_t are supplied by the build */

/* External function definition expected by the benchmark. */
seq_t a(idx_t n)
{
    /* compute the triangular number T(n) */
    seq_t tri = (seq_t)n * (seq_t)(n + 1) / 2;

    /* factor n and count divisors */
    idx_t m = n;
    seq_t d = 1;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* count power of prime p in n */
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* each prime factor contributes (exp+1) to divisor count */
            d *= (seq_t)(exp + 1);
        }
    }

    /* if remaining m > 1, it is a prime factor with exponent 1 */
    if (m > 1) {
        d *= 2;
    }

    /* final result */
    return tri * d;
}
