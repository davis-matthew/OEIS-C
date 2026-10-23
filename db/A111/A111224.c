/*
 * A111224  Numbers n such that 5*n + 7 is prime.
 * https://oeis.org/A111224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that 5*n + 7 is prime
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS offset 1: a(1) = 0, a(2) = 2, …) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;

    /* Search for the n‑th integer m with 5*m+7 prime */
    while (1) {
        uint64_t val = 5ULL * (uint64_t)m + 7ULL;
        if (is_prime(val)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
