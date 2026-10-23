/*
 * A105135  Numbers n such that 32n+17 is prime.
 * https://oeis.org/A105135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth nonnegative integer n for which 32*n+17 is prime */
/* The sequence is indexed from n = 1 and a(1) = 0 as provided in the OEIS data. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Count how many values satisfy the condition until we reach the nth one. */
    idx_t found = 0;
    for (idx_t i = 0; ; ++i) {
        uint64_t val = 32ull * i + 17ull;
        if (is_prime(val)) {
            ++found;
            if (found == n) {
                return (seq_t)i;
            }
        }
    }
}
