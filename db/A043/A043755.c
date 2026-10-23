/*
 * A043755  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 1 mod 9.
 * https://oeis.org/A043755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Search: a(n) is the nth positive integer whose number of
 * runs in its binary representation is congruent to 1 (mod 9).
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t count  = 0;
    uint64_t val = 1;

    for (;; ++val) {
        /* number of binary runs equals popcount(val ^ (val >> 1)) */
        unsigned int runs = __builtin_popcountll(val ^ (val >> 1));

        if ((runs % 9U) == 1U) {
            ++count;
            if (count == target)
                return (seq_t)val;   /* cast to arbitrary‑precision type if needed */
        }
    }
}
