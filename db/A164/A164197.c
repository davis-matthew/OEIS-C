/*
 * A164197  Number of binary strings of length n with equal numbers of 00001
 *          and 01000 substrings.
 * https://oeis.org/A164197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n with equal numbers
 *            of 00001 and 01000 substrings. This implementation uses a
 *            dynamic programming over the last four bits and the current
 *            difference of the two substring counts
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* n < 5: all strings are valid because no 5‑bit substring can exist. */
    if (n < 5) {
        seq_t v = 1;
        for (idx_t i = 0; i < n; ++i) v <<= 1;
        return v;
    }

    idx_t steps = n - 4;                 /* number of extensions after
                                           the initial four bits */
    idx_t maxDiff = 2 * steps + 1;       /* range of possible differences
                                           [-steps, steps] */
    size_t size = 16 * maxDiff;          /* 16 states for last 4 bits */
    seq_t *cur  = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(size, sizeof(seq_t));
    if (!cur || !next) return 0;        /* allocation failure */

    idx_t offset = steps;                /* index of difference == 0 */

    /* initialise with all 16 possible 4‑bit patterns, zero difference */
    for (int s = 0; s < 16; ++s) cur[s * maxDiff + offset] = 1;

    /* iterate over each added bit */
    for (idx_t step = 1; step <= steps; ++step) {
        idx_t d_start = offset - (step - 1);
        idx_t d_end   = offset + (step - 1);

        for (int s = 0; s < 16; ++s) {
            idx_t base = s * maxDiff;

            for (idx_t d = d_start; d <= d_end; ++d) {
                seq_t val = cur[base + d];
                if (!val) continue;

                /* transition with bit 0 */
                {
                    int s2 = (s << 1) & 0xF;          /* shift left, drop 5th bit */
                    int w  = (s == 4) ? -1 : 0;      /* pattern 01000 */
                    idx_t d2 = d + w;
                    next[s2 * maxDiff + d2] += val;
                }
                /* transition with bit 1 */
                {
                    int s2 = ((s << 1) & 0xF) | 1;
                    int w  = (s == 0) ? 1 : 0;       /* pattern 00001 */
                    idx_t d2 = d + w;
                    next[s2 * maxDiff + d2] += val;
                }
            }
        }
        /* swap current and next, then clear next for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        memset(next, 0, size * sizeof(seq_t));
    }

    /* sum the counts that end with difference zero */
    seq_t result = 0;
    for (int s = 0; s < 16; ++s) result += cur[s * maxDiff + offset];

    free(cur);
    free(next);
    return result;
}
