/*
 * A228387  Number of nX5 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally or vertically.
 * https://oeis.org/A228387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with top‑left entry 1 and no
 *            two 1’s adjacent horizontally or vertically. Implemented by
 *            dynamic programming over 5‑bit column masks
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Valid 5‑bit column masks: no vertical adjacency of 1’s. */
static const idx_t MASKS[13] = {
    0x00, /* 00000 */
    0x01, /* 00001 */
    0x02, /* 00010 */
    0x04, /* 00100 */
    0x05, /* 00101 */
    0x08, /* 01000 */
    0x09, /* 01001 */
    0x0A, /* 01010 */
    0x10, /* 10000 */
    0x11, /* 10001 */
    0x12, /* 10010 */
    0x14, /* 10100 */
    0x15  /* 10101 */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n<1 */

    /* dp[i] holds number of ways for current column with mask MASKS[i] */
    seq_t dp_cur[13] = {0};
    seq_t dp_nxt[13] = {0};

    /* Initialize column 0: top‑left must be 1 (bit 0). */
    for (int i = 0; i < 13; ++i) {
        if (MASKS[i] & 0x01) dp_cur[i] = 1;
    }

    /* Iterate columns 1 … n‑1 */
    for (idx_t col = 1; col < n; ++col) {
        for (int j = 0; j < 13; ++j) dp_nxt[j] = 0;   /* reset */

        for (int i = 0; i < 13; ++i) {
            seq_t val = dp_cur[i];
            if (!val) continue;
            for (int j = 0; j < 13; ++j) {
                /* No horizontal adjacency: masks must be disjoint. */
                if ((MASKS[i] & MASKS[j]) == 0)
                    dp_nxt[j] += val;
            }
        }

        /* swap dp_cur and dp_nxt */
        for (int j = 0; j < 13; ++j) dp_cur[j] = dp_nxt[j];
    }

    /* Sum over all masks for the last column */
    seq_t result = 0;
    for (int i = 0; i < 13; ++i) result += dp_cur[i];
    return result;
}
