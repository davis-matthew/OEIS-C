/*
 * A043523  Numbers having three 8's in base 10.
 * https://oeis.org/A043523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_three_eights
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having exactly three 8's in base 10 (OEIS A043523)
   Computation: brute‑force search scanning integer values and counting
   digits. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_three_eights.h"

/* Returns the n‑th integer that contains exactly three digits 8 (n >= 1). */
seq_t a(idx_t n)
{
    idx_t seen = 0;
    for (uint64_t num = 888ULL; ; ++num) {
        if (has_three_eights(num)) {
            ++seen;
            if (seen == n)
                return (seq_t)num;
        }
    }
}
