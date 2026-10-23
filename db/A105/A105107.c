/*
 * A105107  Numbers n such that 10000n + 1001 is prime.
 * https://oeis.org/A105107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 8511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105107: numbers n such that 10000*n + 1001 is prime. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        uint64_t val = (uint64_t)m * 10000ULL + 1001ULL;
        if (is_prime(val)) {
            if (++count == n)
                return (seq_t)m;
        }
    }
}
