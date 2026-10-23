/*
 * A104194  a(n) = phi(n) - lambda(n).
 * https://oeis.org/A104194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) - lambda(n)
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n >= 1 */
#include <stdint.h>
#include "../utils/gcd_local.h"

/* Carmichael lambda is the lcm of p^(e-1)*(p-1) over all prime powers
   p^e || n.  For p = 2 we use the special cases:
        λ(2)   = 1
        λ(4)   = 2
        λ(2^e) = 2^(e-2)  for e ≥ 3   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* no definition, but guard             */
    uint64_t temp = n;

    seq_t phi   = (seq_t)n;          /* start with φ(n) = n                 */
    seq_t lambda = 1;                /* cumulative lcm of parts             */

    /* Handle power of 2 */
    if ((temp & 1ULL) == 0) {        /* temp is even                         */
        unsigned e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        /* φ: divide once by 2, (p-1)=1 keeps φ unchanged except the factor 2^(e-1) */
        phi = phi / 2;

        /* λ-part for 2^e */
        seq_t part;
        if (e == 1)          part = 1;
        else if (e == 2)     part = 2;
        else                 part = ((seq_t)1) << (e - 2);

        seq_t g = gcd_local(lambda, part);
        lambda = lambda / g * part;
    }

    /* Process odd prime factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* φ: only one factor of (1-1/p) per distinct prime */
            seq_t p_seq = (seq_t)p;
            phi = phi / p_seq * (p_seq - 1);

            /* λ-part for p^e: p^(e-1)*(p-1) */
            seq_t part = p_seq - 1;
            for (unsigned i = 1; i < e; ++i) part *= p_seq;

            seq_t g = gcd_local(lambda, part);
            lambda = lambda / g * part;
        }
    }

    /* Remaining prime factor > sqrt(original n), if any */
    if (temp > 1) {
        uint64_t p = temp;
        seq_t p_seq = (seq_t)p;

        phi = phi / p_seq * (p_seq - 1);

        seq_t part = p_seq - 1;       /* e = 1 */
        seq_t g = gcd_local(lambda, part);
        lambda = lambda / g * part;
    }

    return phi - lambda;
}
