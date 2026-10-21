/*
 * A095762  Numbers whose name in English contains an "h".
 * https://oeis.org/A095762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   contains_h
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose name in English contains an "h" */
/* Implementation: brute‑force search using simple linguistic rules */
#include <stdbool.h>
#include "../utils/contains_h.h"

seq_t a(idx_t n)
{
    idx_t count = 0;

    for (idx_t i = 1; ; ++i) {
        if (contains_h(i)) {
            ++count;
            if (count == n) {
                return (seq_t)i;
            }
        }
    }
}
