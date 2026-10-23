/*
 * A153347  Numbers n>0 such that 7*n-4 is not prime.
 * https://oeis.org/A153347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth positive integer m such that 7*m-4 is NOT prime */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        uint64_t val = 7ull * (uint64_t)m - 4ull;
        if (!is_prime_small(val)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
