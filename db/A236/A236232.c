/*
 * A236232  Number of binary words of length n with exactly 3 (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n.
 * https://oeis.org/A236232
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary words of length n with exactly 3
 *            (possibly overlapping) occurrences of the binary expansion of n
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 38, u64 n <= 70, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Only defined for n >= 7; return 0 otherwise */
    if (n < 7) return 0;

    /* Determine pattern length L (number of bits in n) */
    idx_t L = 0;
    for (idx_t t = n; t != 0; t >>= 1) ++L;

    /* Extract pattern bits: pat[0] = most significant bit */
    uint8_t pat[64];
    for (idx_t i = 0; i < L; ++i)
        pat[i] = (n >> (L - 1 - i)) & 1;

    /* Compute prefix function pi for pattern */
    idx_t pi[65];
    pi[0] = 0;
    for (idx_t i = 1; i < L; ++i) {
        idx_t j = pi[i - 1];
        while (j > 0 && pat[j] != pat[i]) j = pi[j - 1];
        if (pat[j] == pat[i]) ++j;
        pi[i] = j;
    }
    idx_t failL = pi[L - 1];   /* shift after a match */

    /* Build transition table and occurrence increment table */
    idx_t next_state[65][2];
    uint8_t inc[65][2];
    for (idx_t s = 0; s < L; ++s) {
        for (int b = 0; b < 2; ++b) {
            idx_t j = s;
            while (j > 0 && pat[j] != b) j = pi[j - 1];
            if (pat[j] == b) ++j;
            if (j == L) {
                inc[s][b] = 1;
                next_state[s][b] = failL;
            } else {
                inc[s][b] = 0;
                next_state[s][b] = j;
            }
        }
    }

    /* DP over positions: curr[state][cnt] = ways */
    seq_t curr[65][4] = {{0}};
    seq_t next[65][4];
    curr[0][0] = 1;       /* start with empty word, zero matches */

    for (idx_t pos = 0; pos < n; ++pos) {
        /* zero next layer */
        for (idx_t s = 0; s < L; ++s)
            for (int c = 0; c < 4; ++c)
                next[s][c] = 0;

        for (idx_t s = 0; s < L; ++s) {
            for (int c = 0; c < 4; ++c) {
                seq_t ways = curr[s][c];
                if (ways == 0) continue;
                for (int b = 0; b < 2; ++b) {
                    idx_t ns = next_state[s][b];
                    int nc = c + inc[s][b];
                    if (nc <= 3)
                        next[ns][nc] += ways;
                }
            }
        }
        /* copy next to curr */
        for (idx_t s = 0; s < L; ++s)
            for (int c = 0; c < 4; ++c)
                curr[s][c] = next[s][c];
    }

    /* Sum over all end states with exactly 3 occurrences */
    seq_t result = 0;
    for (idx_t s = 0; s < L; ++s)
        result += curr[s][3];

    return result;
}
