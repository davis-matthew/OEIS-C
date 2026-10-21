/*
 * A097295  Length of longest non-overlapping repeated subword of n in binary
 *          representation.
 * https://oeis.org/A097295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   subvalue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by exhaustive search of non‑overlapping repeated subwords
   of the binary representation of n; only substrings with a leading
   '1' are considered when the subword length is > 1. */

#include <stdint.h>
#include "../utils/subvalue.h"

seq_t a(idx_t n)
{
    /* Compute the number of bits of n. */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp) { ++L; tmp >>= 1; }
    if (L < 2) return 0;            /* no repeated subword possible */

    /* Try decreasing lengths from floor(L/2) down to 1. */
    for (idx_t l = L / 2; l >= 1; --l) {
        for (idx_t i = 0; i + l <= L; ++i) {
            /* For lengths > 1, skip substrings that start with 0. */
            if (l > 1) {
                idx_t msb_pos = L - 1 - i;      /* position of the first bit of the substring */
                if (!((n >> msb_pos) & 1)) continue;
            }
            seq_t vi = subvalue(n, L, l, i);

            for (idx_t j = i + l; j + l <= L; ++j) {
                if (l > 1) {
                    idx_t msb_pos_j = L - 1 - j;
                    if (!((n >> msb_pos_j) & 1)) continue;
                }
                if (vi == subvalue(n, L, l, j))
                    return (seq_t)l;          /* longest length found */
            }
        }
    }
    return 0;
}
