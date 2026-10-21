/*
 * A226602  Number of ordered triples (i,j,k) with i*j*k = n, i,j,k >= 0 and
 *          gcd(i,j,k) <= 1.
 * https://oeis.org/A226602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 for n=0; otherwise a(n) = ∏_{p^e‖n} 3*e , where the
 *            product is over the distinct prime factors of n
 * Range:     u8 n <= 1259, u16 n <= 9610, u32 n <= 9610, u64 n <= 9610,
 *            u128 n <= 9610, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    idx_t temp = n;
    seq_t prod = (seq_t)1;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            prod *= (seq_t)3 * (seq_t)exp;
        }
    }
    if (temp > 1) {
        prod *= (seq_t)3;   /* exponent of the remaining prime is 1 */
    }
    return prod;
}
