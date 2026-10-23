/*
 * A119653  Denominator of BernoulliB[2p] divided by 6, where p=Prime[n].
 * https://oeis.org/A119653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator(BernoulliB[2Prime[n]]) / 6 The denominator of
 *            B_{2p} equals the product of all primes q for which q‑1 divides
 *            2p. For a prime p, the divisors of 2p are {1,2,p,2p} and
 *            consequently the relevant primes are {2,3,p+1,2p+1}. Each
 *            appears at most once; the product is therefore 2·3 an...
 * Range:     u8 n <= 8, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, return 0 as a placeholder */

    /* obtain the n-th prime p */
    seq_t p = (seq_t)get_prime(n);

    /* start with the primes 2 and 3 that always divide the denominator */
    seq_t prod = (seq_t)2 * (seq_t)3;  /* product = 6 */

    /* candidate prime q = p + 1 */
    seq_t q1 = p + (seq_t)1;
    if (is_prime_big(q1)) {
        /* q1 can only be 3 when p == 2, already multiplied above */
        if (q1 != (seq_t)2 && q1 != (seq_t)3) {
            prod *= q1;
        }
    }

    /* candidate prime q = 2p + 1 */
    seq_t q2 = (p << 1) + (seq_t)1;  /* 2*p + 1 */
    if (is_prime_big(q2)) {
        /* q2 > 3 for all primes p, so no duplication with 2 or 3 */
        prod *= q2;
    }

    /* divide by 6 (2·3) to obtain the sequence value */
    return prod / (seq_t)6;
}
