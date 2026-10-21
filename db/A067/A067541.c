/*
 * A067541  a(n) = phi(n*(n+1)/2)/phi(n) where phi is the Euler totient
 *          function A000010(n).
 * https://oeis.org/A067541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n*(n+1)/2)/phi(n) Efficient computation: Let n = 2^k
 *            * m with gcd(m,2)=1. Then a(n) = phi((n+1)/2) if k == 0,
 *            phi(n+1) if k == 1, phi(n+1)/2 if k > 1
 * Range:     u8 n <= 261, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   exponent_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/exponent_of_two.h"

/* Return the value of Euler's totient function φ(m).  */
static seq_t compute_phi(idx_t m)
{
    idx_t result = m;
    idx_t p;

    for (p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
    }
    if (m > 1)
        result -= result / m;

    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by the offset of the sequence. */
    int k = exponent_of_two(n);

    if (k == 0) {                       /* n odd */
        idx_t half = (n + 1) / 2;
        return compute_phi(half);
    } else if (k == 1) {                /* exactly one factor of 2 */
        return compute_phi(n + 1);
    } else {                            /* at least two factors of 2 */
        return compute_phi(n + 1) / 2U;
    }
}
