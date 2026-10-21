/*
 * A228325  a(n) is the smallest number m>n such that the concatenation nm is
 *          prime.
 * https://oeis.org/A228325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m > n such that concatenation of n and m is
 *            prime
 * Range:     u8 n <= 8, u16 n <= 866, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, is_prime_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* safety for offset 1 */
    uint64_t m = (uint64_t)n + 1;

    while (1) {
        uint64_t digits_m = digit_count(m);
        uint64_t concat = (uint64_t)n * (uint64_t)pow10(digits_m) + m;
        if (is_prime_small(concat))
            return (seq_t)m;
        ++m;
    }
}
