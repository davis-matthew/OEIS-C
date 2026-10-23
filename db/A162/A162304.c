/*
 * A162304  Triangular numbers that contain exactly 3 different decimal
 *          digits.
 * https://oeis.org/A162304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth triangular number that contains exactly 3
 *            different decimal digits
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n) {
    seq_t tri = 0;
    idx_t k = 1;
    idx_t count = 0;
    unsigned char cnt[10];   /* counts of digits 0–9 in the current triangular number */

    while (count < n) {
        /* T_k = T_{k-1} + k  (k starts at 1 so T_1 = 1) */
        tri += (seq_t)k;

        digit_counts(tri, cnt);

        int distinct = 0;
        for (int d = 0; d < 10; ++d)
            if (cnt[d]) ++distinct;

        if (distinct == 3) {
            ++count;
            if (count == n)
                return tri;
        }
        ++k;
    }
    /* The loop always returns a value; this line is never reached. */
    return tri;
}
