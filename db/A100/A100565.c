/*
 * A100565  a(n) = Card{(x,y,z) : x <= y <= z, x|n, y|n, z|n, gcd(x,y)=1,
 *          gcd(x,z)=1, gcd(y,z)=1}.
 * https://oeis.org/A100565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (tau(n^3) + 3*tau(n) + 2)/6
 */
#include <stdint.h>

/* returns a(n) as defined for the OEIS sequence A100565 */
seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n <= 1u) return (seq_t)1;

    /* factor n and accumulate the divisor counts */
    uint64_t nn = (uint64_t)n;
    seq_t tau_n   = (seq_t)1;  /* tau(n)   = product (e_i + 1)      */
    seq_t tau_n3  = (seq_t)1;  /* tau(n³)  = product (3*e_i + 1)  */

    /* factor 2 separately */
    int e = 0;
    while ((nn & 1ULL) == 0ULL) {
        nn >>= 1;
        ++e;
    }
    if (e > 0) {
        tau_n   *= (seq_t)(e + 1);
        tau_n3  *= (seq_t)(3 * e + 1);
    }

    /* trial division by odd numbers */
    for (uint64_t p = 3ULL; p * p <= nn; p += 2ULL) {
        if (nn % p == 0ULL) {
            e = 0;
            while (nn % p == 0ULL) {
                nn /= p;
                ++e;
            }
            tau_n  *= (seq_t)(e + 1);
            tau_n3 *= (seq_t)(3 * e + 1);
        }
    }

    /* if a prime factor remains */
    if (nn > 1ULL) {
        tau_n  *= (seq_t)2; /* exponent 1 -> e+1 = 2 */
        tau_n3 *= (seq_t)4; /* exponent 1 -> 3*1+1 = 4 */
    }

    /* compute the final formula */
    seq_t numerator = tau_n3 + (tau_n * (seq_t)3) + (seq_t)2;
    return numerator / (seq_t)6;
}
