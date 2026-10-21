/*
 * A219084  Number of 7 X n binary arrays with every 0 a horizontal, diagonal
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A219084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP counting of 7×n binary arrays where every 0 has a horizontal,
   diagonal or antidiagonal neighbor to some 1.  */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define ROWS     7
#define STATES  (1 << ROWS)           /* 128 */
#define ALLMASK (STATES - 1)          /* 0x7f */

/* Pre‑computed masks */
static uint8_t coverMask[STATES];
static uint8_t zerosMask[STATES];

/* DP tables */
static seq_t dp_prev[STATES][STATES];
static seq_t dp_next[STATES][STATES];

/* Flag for one‑time initialization */
static bool init_done = false;

seq_t a(idx_t n) {
    /* one‑time init of static tables */
    if (!init_done) {
        init_done = true;
        for (int p = 0; p < STATES; ++p) {
            uint8_t m = (uint8_t)p;
            coverMask[p] = (m | (m << 1) | (m >> 1)) & ALLMASK;
            zerosMask[p] = (~m) & ALLMASK;
        }
    }

    /* clear dp_prev for this call */
    for (int p = 0; p < STATES; ++p)
        memset(dp_prev[p], 0, sizeof(dp_prev[p]));

    /* initialise dp_prev for column 1 */
    for (int p = 0; p < STATES; ++p) {
        int req = (~p) & ALLMASK;          /* all zeros need right neighbour */
        dp_prev[p][req] = (seq_t)1;
    }

    if (n == 1) {
        seq_t ans = 0;
        for (int p = 0; p < STATES; ++p)
            ans += dp_prev[p][0];
        return ans;
    }

    /* temporary array for subset sums */
    seq_t subset[STATES];

    for (idx_t col = 2; col <= n; ++col) {
        /* clear dp_next */
        for (int p = 0; p < STATES; ++p)
            memset(dp_next[p], 0, sizeof(dp_next[p]));

        for (int prev = 0; prev < STATES; ++prev) {
            /* copy current row into subset array */
            for (int m = 0; m < STATES; ++m)
                subset[m] = dp_prev[prev][m];

            /* subset sum transform: subset[mask] = sum_{sub ⊆ mask} dp_prev[prev][sub] */
            for (int bit = 0; bit < ROWS; ++bit) {
                for (int m = 0; m < STATES; ++m) {
                    if (m & (1 << bit))
                        subset[m] += subset[m ^ (1 << bit)];
                }
            }

            uint8_t leftCover = coverMask[prev];
            for (int next = 0; next < STATES; ++next) {
                uint8_t coverNext = coverMask[next];
                seq_t count = subset[coverNext];
                if (count == (seq_t)0)
                    continue;

                uint8_t zerosNext = zerosMask[next];
                uint8_t newReq = zerosNext & (~leftCover);
                newReq &= ALLMASK;   /* keep only 7 bits */
                dp_next[next][newReq] += count;
            }
        }

        /* swap dp_prev and dp_next */
        for (int p = 0; p < STATES; ++p)
            memcpy(dp_prev[p], dp_next[p], sizeof(dp_prev[p]));
    }

    /* final answer: pending mask must be zero after the last column */
    seq_t ans = 0;
    for (int p = 0; p < STATES; ++p)
        ans += dp_prev[p][0];
    return ans;
}
