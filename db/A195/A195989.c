/*
 * A195989  Quotient of denominators of (BernoulliB(2n)/n) and BernoulliB(2n).
 * https://oeis.org/A195989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (product of all prime powers p^e | 2n with p‑1 | 2n) / 2
 *            Here the product is taken over the unique prime powers p^e
 *            contained in the factorisation of 2n. The result is equivalent
 *            to the quotient of denominators of BernoulliB(2n)/n and
 *            BernoulliB(2n)
 * Range:     u8 n <= 255, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used – offset starts at 1 */

    /* 2n must fit in 64 bits for the simple factoriser below.
       The test data keeps n <= 10000, so this is safe. */
    uint64_t m = n * 2ULL;          /* m = 2*n */
    seq_t prod = (seq_t)1;          /* multiplicative accumulator */

    uint64_t tmp = m;

    /* trial division up to sqrt(tmp) */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            /* include p^exp iff (p-1) divides m */
            if (m % (p - 1) == 0) {
                for (unsigned i = 0; i < exp; ++i) {
                    prod = prod * (seq_t)p;
                }
            }
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        uint64_t p = tmp;
        if (m % (p - 1) == 0) {
            prod = prod * (seq_t)p;   /* exponent is 1 */
        }
    }

    /* 2a(n) equals the product; thus a(n) = product / 2 */
    return prod / 2;
}
