/*
 * A082293  Numbers having exactly one square divisor > 1.
 * https://oeis.org/A082293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_one_square_divisor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth integer that has exactly one square divisor > 1 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_one_square_divisor_u64.h"

/* Main sequence function.  For any order of calls, we keep an internal
 * counter only to find the nth term on the fly; no caching is used,
 * which keeps the code portable to all build types (including big‑int). */
seq_t a(idx_t n)
{
    /* n>=1 guarantees that the loop will hit the first term 4. */
    uint64_t count = 0;
    uint64_t cand   = 4;          /* first possible integer with a square divisor >1 */

    while (1) {
        if (has_one_square_divisor_u64(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
