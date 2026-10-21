/*
 * A202465  Number of (n+2)X4 binary arrays with no more than two of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+2)×4 binary arrays with no more than two *
 *            consecutive ones in any row or column (i.e., no “111”
 *            horizontally or * vertically). Computed by a row‑by‑row dynamic
 *            program over the * column states
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Transition table: for each 8‑bit state (two bits per column) give
     * all reachable next states when a row that contains no “111” is added. */
    static int trans_next[256][13];
    static unsigned char trans_cnt[256];
    static int built = 0;

    if (!built) {
        /* All 4‑bit row patterns that have no “111” substring. */
        const uint8_t patterns[13] = {
            0u, 1u, 2u, 3u, 4u, 5u, 6u,
            8u, 9u, 10u, 11u, 12u, 13u
        };

        for (int s = 0; s < 256; ++s) {
            unsigned char count = 0;
            int prev2[4], prev1[4];
            for (int j = 0; j < 4; ++j) {
                int code = (s >> (2 * j)) & 3;   /* two bits per column */
                prev2[j] = code >> 1;           /* most recent bit */
                prev1[j] = code & 1;            /* second‑most recent */
            }

            for (int pi = 0; pi < 13; ++pi) {
                uint8_t pat = patterns[pi];
                int new_s = 0;
                int ok = 1;
                for (int j = 0; j < 4; ++j) {
                    int new_bit = (pat >> j) & 1;
                    /* Check vertical constraint: three consecutive 1's forbidden. */
                    if (prev2[j] && prev1[j] && new_bit) {
                        ok = 0;
                        break;
                    }
                    /* Encode new state: shift the previous bits. */
                    int new_code = (prev1[j] << 1) | new_bit;
                    new_s |= new_code << (2 * j);
                }
                if (ok) {
                    trans_next[s][count++] = new_s;
                }
            }
            trans_cnt[s] = count;
        }
        built = 1;
    }

    /* Number of rows is (n+2). */
    idx_t rows = n + 2;
    seq_t cur[256] = {0};
    seq_t nxt[256] = {0};

    /* Initial state: no previous bits. */
    cur[0] = 1;

    for (idx_t r = 0; r < rows; ++r) {
        /* Clear next array. */
        for (int i = 0; i < 256; ++i) {
            nxt[i] = 0;
        }

        /* Transition to the next row. */
        for (int s = 0; s < 256; ++s) {
            seq_t val = cur[s];
            if (!val) continue;
            unsigned char cnt = trans_cnt[s];
            for (unsigned char t = 0; t < cnt; ++t) {
                int ns = trans_next[s][t];
                nxt[ns] += val;
            }
        }

        /* Move to next iteration. */
        for (int i = 0; i < 256; ++i) {
            cur[i] = nxt[i];
        }
    }

    /* Sum all final states. */
    seq_t result = 0;
    for (int i = 0; i < 256; ++i) {
        result += cur[i];
    }
    return result;
}
