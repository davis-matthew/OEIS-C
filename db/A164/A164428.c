/*
 * A164428  Number of binary strings of length n with no substrings equal to
 *          0000, 0011, or 1001.
 * https://oeis.org/A164428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n avoiding *
 *            substrings 0000, 0011, or 1001. * This implementation uses
 *            dynamic programming on the last three bits * of the string. For
 *            lengths below 4 all strings are allowed. For * lengths ≥ 4, when
 *            adding a new bit we form the 4‑bit window consisting *...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 84, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t curr[8] = {0};
    seq_t next[8];
    curr[0] = 1;              /* start with the empty string */

    for (idx_t pos = 0; pos < n; ++pos) {
        /* clear the next array */
        for (int i = 0; i < 8; ++i) next[i] = 0;

        for (int mask = 0; mask < 8; ++mask) {
            seq_t val = curr[mask];
            if (val == 0) continue;

            for (int b = 0; b < 2; ++b) {
                int newmask = ((mask << 1) | b) & 7;
                if (pos + 1 >= 4) {
                    seq_t word = ((seq_t)mask << 1) | b;
                    /* forbidden words: 0000 (0), 0011 (3), 1001 (9) */
                    if (word == 0 || word == 3 || word == 9) continue;
                }
                next[newmask] += val;
            }
        }

        /* swap curr and next for the next iteration */
        for (int i = 0; i < 8; ++i) curr[i] = next[i];
    }

    /* Sum over all possible last‑three‑bit states */
    seq_t result = 0;
    for (int i = 0; i < 8; ++i) result += curr[i];
    return result;
}
