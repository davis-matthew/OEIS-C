/*
 * A047819  a(n) = Product_{i=1..n} ((i+3)*(i+4)*(i+5))/(i*(i+1)*(i+2)).
 * https://oeis.org/A047819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..n} ((i+3)(i+4)(i+5))/(i(i+1)(i+2))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 29, u64 n <= 375,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(n) = C(n+3,1) * C(n+4,3) * C(n+5,5) / 12  */
    seq_t denom = 12;
    seq_t res   = 1;

    /* factor C(n+3,1) = n+3 */
    seq_t r1 = binomial(n + 3, 1);
    seq_t g = gcd(r1, denom);
    r1 /= g;
    denom /= g;
    res *= r1;

    /* factor C(n+4,3) */
    seq_t r2 = binomial(n + 4, 3);
    g = gcd(r2, denom);
    r2 /= g;
    denom /= g;
    res *= r2;

    /* factor C(n+5,5) */
    seq_t r3 = binomial(n + 5, 5);
    g = gcd(r3, denom);
    r3 /= g;
    denom /= g;
    res *= r3;

    /* Now denom must be 1 */
    return res;
}
