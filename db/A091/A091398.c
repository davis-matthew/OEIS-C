/*
 * A091398  a(n) = Product_{ p | n } (1 + Legendre(5,p) ).
 * https://oeis.org/A091398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p|n} (1 + Legendre(5,p))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) /* Undefined for n = 0, but return 0 safely */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Even numbers contain the prime 2 → Legendre(5,2) = –1 → product 0 */
    if ((n & 1) == 0)
        return (seq_t)0;

    idx_t m = n;
    seq_t result = (seq_t)1;

    /* trial division for odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            /* p is a distinct prime divisor of n */
            if (p != 5) {
                idx_t r = p % 5;
                if (r == 1 || r == 4) {
                    result *= (seq_t)2;            /* factor 2 */
                } else {                            /* r == 2 or 3 → non‑residue */
                    return (seq_t)0;                /* factor 0 */
                }
            }
            /* Remove all powers of this prime */
            while (m % p == 0)
                m /= p;
        }
    }

    /* Remaining prime factor greater than sqrt(n) */
    if (m > 1) {                     /* m is prime */
        if (m != 5) {
            idx_t r = m % 5;
            if (r == 1 || r == 4) {
                result *= (seq_t)2;
            } else {
                return (seq_t)0;
            }
        }
    }

    return result;
}
