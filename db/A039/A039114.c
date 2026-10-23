/*
 * A039114  Numbers whose base-10 representation has the same number of 0's
 *          and 3's.
 * https://oeis.org/A039114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose decimal representation has the same number of 0's
 *            and 3's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_counts.h"

static bool equal_zero_three(seq_t v) {
    unsigned char cnt[10];
    digit_counts(v, cnt);
    return cnt[0] == cnt[3];
}

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t val   = 0;
    while (found < n) {
        val++;
        if (equal_zero_three(val)) {
            found++;
        }
    }
    return val;
}
