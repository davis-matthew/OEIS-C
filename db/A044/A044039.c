/*
 * A044039  Numbers k such that 7 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_79_or_97
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044039: numbers containing the adjacent digits "79" or "97" but not in the successor.
   This implementation finds the n‑th term by a simple brute‑force search. */

#include <stdbool.h>
#include "../utils/has_79_or_97.h"

seq_t a(idx_t n) {
    seq_t k = 1;
    idx_t count = 0;
    while (1) {
        if (has_79_or_97(k)) {
            if (!has_79_or_97(k + 1)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
        ++k;
    }
}
