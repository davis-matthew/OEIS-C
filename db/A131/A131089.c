/*
 * A131089  a(n) = Sum_{d|n} (2 - mu(d)).
 * https://oeis.org/A131089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (2 - mu(d)). For n>1 this simplifies to a(n) =
 *            2*tau(n), with tau(n) the number of divisors of n. a(1)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;                     /* a(1) = 1 */

    /* Compute tau(n) = product over prime exponents (e+1) */
    idx_t m = n;
    seq_t tau = (seq_t)1;

    /* Handle factor 2 */
    idx_t exp = (idx_t)0;
    while ((m & (idx_t)1) == (idx_t)0) {    /* m % 2 == 0 */
        ++exp;
        m >>= 1;
    }
    if (exp > (idx_t)0)
        tau *= (exp + (idx_t)1);

    /* Handle odd factors */
    for (idx_t p = (idx_t)3; p * p <= m; p += (idx_t)2) {
        exp = (idx_t)0;
        while (m % p == (idx_t)0) {
            ++exp;
            m /= p;
        }
        if (exp > (idx_t)0)
            tau *= (exp + (idx_t)1);
    }

    /* If remaining m > 1, it is a prime factor with exponent 1 */
    if (m > (idx_t)1)
        tau *= (idx_t)2;

    /* a(n) = 2 * tau(n) */
    return tau * (seq_t)2;
}
