/*
 * A100549  Let n = 2^e_2 * 3^e_ * 5^e_ * ... be the prime factorization of n;
 *          then a(n) = largest prime <= 1 + max{e_2, e_3, e_5, ...}; a(1) = 1
 *          by convention.
 * https://oeis.org/A100549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime <= 1 + max{exponent of each prime in n};
 *            a(1) = 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Special case for n = 1 */
    if (n == 1) return 1;

    /* Factor n and find the maximum exponent */
    uint64_t nn = (uint64_t)n;
    uint64_t max_exp = 0;
    uint64_t tmp = nn;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (e > max_exp) max_exp = e;
        }
    }
    /* Remaining prime factor greater than sqrt(n) */
    if (tmp > 1 && 1 > max_exp) max_exp = 1;

    /* Search for the largest prime <= 1 + max_exp */
    uint64_t limit = max_exp + 1;
    for (uint64_t x = limit; x >= 2; --x) {
        if (is_prime(x)) {
            return (seq_t)x;
        }
    }
    /* Fallback: should never occur for n > 1 */
    return 2;
}
