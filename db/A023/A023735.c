/*
 * A023735  Numbers with exactly 2 3's in their base-5 expansion.
 * https://oeis.org/A023735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   two_threes_in_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A023735: Numbers with exactly 2 3's in base‑5 expansion */
#include <stdbool.h>
#include "../utils/two_threes_in_base5.h"

seq_t a(idx_t n) {
    seq_t val = 0;
    idx_t found = 0;
    while (found < n) {
        if (two_threes_in_base5(val)) {
            ++found;
            if (found == n) {
                return val;
            }
        }
        ++val;
    }
    return val; /* unreachable */
}
