/*
 * A117802  Numbers with an "a" in Dutch.
 * https://oeis.org/A117802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   numbers whose Dutch name contains the letter 'a'
 * Range:     u8 n <= 44, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   contains_a_dutch, part_has_a_small, scale_has_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/contains_a_dutch.h"
#include "../utils/part_has_a_small.h"
#include "../utils/scale_has_a.h"

/* true if the scale (thousands, million, etc.) name contains 'a' */

/* Return the n-th term of A117802 (n >= 1) */
seq_t a(idx_t n)
{
    idx_t count    = 0;
    seq_t candidate = 1;
    while (1) {
        if (contains_a_dutch(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
}
