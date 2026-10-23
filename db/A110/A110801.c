/*
 * A110801  Numbers k such that 12k + 1 is prime.
 * https://oeis.org/A110801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that 12*k + 1 is prime
 * Range:     u8 n <= 103, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t n) */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour for n < 1 is irrelevant */
        return (seq_t)0;

    idx_t count = 0;
    idx_t k = 1;

    for (;; ++k)
    {
        uint64_t candidate = 12ULL * (uint64_t)k + 1ULL;
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
