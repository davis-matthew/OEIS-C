/*
 * A039023  Numbers whose base-6 representation has the same number of 1's and
 *          5's.
 * https://oeis.org/A039023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th integer whose base‑6 representation contains an
 *            equal number of 1’s and 5’s (computed by brute‑force search)
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal15.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t val = 0;
    while (1) {
        if (equal15(val)) {
            ++found;
            if (found == n) return val;
        }
        val = val + 1;
    }
}
