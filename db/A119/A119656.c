/*
 * A119656  Denominator of BernoulliB(4*prime(n))/30.
 * https://oeis.org/A119656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator(BernoulliB(4*prime(n)))/30
 * Range:     u8 n <= 1, u16 n <= 299, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Get the n‑th prime; n is 1‑based. */
    uint32_t p = (uint32_t)get_prime(n);

    /* 4·p fits in 64 bits for all arguments that occur in the tests. */
    uint64_t v = 4ULL * (uint64_t)p;

    seq_t prod = 1;

    /* Enumerate all divisors d of v and test d+1 for primality. */
    for (uint64_t d = 1; d * d <= v; ++d) {
        if (v % d == 0) {
            uint64_t q1 = d + 1;
            if (is_prime_small((seq_t)q1))
                prod = prod * (seq_t)q1;

            uint64_t d2 = v / d;
            if (d2 != d) {
                uint64_t q2 = d2 + 1;
                if (is_prime_small((seq_t)q2))
                    prod = prod * (seq_t)q2;
            }
        }
    }

    /* Divide by 30 (2·3·5) – guaranteed to divide evenly. */
    prod = prod / (seq_t)30;

    return prod;
}
