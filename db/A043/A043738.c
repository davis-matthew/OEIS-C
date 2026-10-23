/*
 * A043738  Numbers whose number of runs in the base-2 representation is
 *          congruent to 5 mod 6.
 * https://oeis.org/A043738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   runs_congruent_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th integer whose number of runs in the binary representation
   is congruent to 5 modulo 6 (OEIS A043738).  The implementation performs a
   simple brute–force search, testing each integer until the required term
   is found.  The test for the run condition uses only integer arithmetic
   and works for every build of seq_t. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/runs_congruent_five.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for the sequence, but safe */

    uint64_t cnt = 0;
    uint64_t x = 1;

    while (cnt < (uint64_t)n) {
        if (runs_congruent_five(x)) {
            ++cnt;
            if (cnt == (uint64_t)n)
                return (seq_t)x;
        }
        ++x;
    }

    /* We should never reach here for valid n. */
    return (seq_t)0;
}
