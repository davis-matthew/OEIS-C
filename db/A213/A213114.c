/*
 * A213114  Number of binary arrays of length n+7 with fewer than 4 ones in
 *          any length 8 subsequence (=less than 50% duty cycle).
 * https://oeis.org/A213114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n+7 such that every 8
 *            consecutive bits contain at most 3 ones. The computation uses a
 *            linear‑time DP over the last 7 bits
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 36, u64 n <= 80, u128 n <= 169,
 *            big any n
 * Helpers:   popcount8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External helper: none */
/* External types: idx_t (e.g. uint64_t), seq_t (unsigned integer or big int) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/popcount8.h"

seq_t a(idx_t n) {
    /* Length of the binary array */
    idx_t L = n + (idx_t)7;

    /* Two DP tables for the last 7 bits (128 possible masks). */
    seq_t out[128] = {0};
    seq_t tmp[128] = {0};
    seq_t *prev = out;
    seq_t *curr = tmp;

    prev[0] = (seq_t)1;               /* empty string */

    for (idx_t pos = 0; pos < L; ++pos) {
        /* zero current table */
        for (int i = 0; i < 128; ++i) curr[i] = 0;

        for (int mask = 0; mask < 128; ++mask) {
            seq_t val = prev[mask];
            if (!val) continue;

            for (int bit = 0; bit <= 1; ++bit) {
                if (pos >= 7) {
                    unsigned int window = ((unsigned int)mask << 1) | (unsigned int)bit;
                    if (popcount8(window) > 3) continue;
                }
                int newmask = (((mask << 1) | bit) & 0x7F);
                curr[newmask] += val;
            }
        }

        /* swap tables for next iteration */
        seq_t *swap = prev;
        prev = curr;
        curr = swap;
    }

    /* Sum all ending states to get the final count */
    seq_t result = 0;
    for (int mask = 0; mask < 128; ++mask) result += prev[mask];

    return result;
}
