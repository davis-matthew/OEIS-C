/*
 * A039126  Numbers whose base-10 representation has the same number of 1's
 *          and 8's.
 * https://oeis.org/A039126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_counts.h"

/* A039126: Brute force search for numbers whose decimal representation has equal numbers of 1's and 8's */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t cand  = 0;
    unsigned char cnt[10];
    while (1) {
        digit_counts(cand, cnt);
        if (cnt[1] == cnt[8]) {
            count++;
            if (count == n) return cand;
        }
        cand++;
    }
}
