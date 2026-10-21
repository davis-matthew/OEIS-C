/*
 * A078828  Product of all primes contained as binary substrings in binary
 *          representation of n.
 * https://oeis.org/A078828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of all prime numbers that appear as consecutive
 *            binary substrings of the binary representation of n, counting
 *            each occurrence. Substrings that start with a zero bit are
 *            ignored. a(0) = 1
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* n = 0 -> product = 1 */
    if (n == 0)
        return (seq_t)1;

    /* number of bits in n (>=1).  Using the GCC builtin __builtin_clzll. */
    unsigned L = 64 - __builtin_clzll(n);

    seq_t prod = (seq_t)1;

    for (unsigned i = 0; i < L; ++i) {
        /* skip substrings whose first bit is 0 (leading zero) */
        if (!((n >> (L - 1 - i)) & 1ULL))
            continue;

        seq_t val = (seq_t)0;
        for (unsigned j = i; j < L; ++j) {
            unsigned bit = (n >> (L - 1 - j)) & 1ULL;
            val = (val << 1) | bit;

            if (val > (seq_t)1 && is_prime_small(val))
                prod = prod * val;
        }
    }
    return prod;
}
