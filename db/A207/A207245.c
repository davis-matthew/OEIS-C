/*
 * A207245  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 18, u64 n <= 45, u128 n <= 97,
 *            big any n
 * Helpers:   horiz_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: 5×n binary arrays avoiding horizontal patterns
   0 0 0 and 0 1 1, and avoiding vertical patterns 0 0 1 and 0 1 0.
   Counting is done by a transfer‑matrix dynamic program over the 14
   column states that satisfy the vertical restriction.  The DP
   keeps the last two columns; transitions are pre‑computed once.  */

/* required external types */
#include <stdbool.h>
#include "../utils/horiz_ok.h"

#define NUM_COLS 14
#define COL_MASK 0x1F     /* 5 bits */

static uint8_t cols[NUM_COLS];                     /* allowed column patterns   */
static int next_cnt[NUM_COLS * NUM_COLS];          /* allowed next columns count*/
static idx_t next_list[NUM_COLS * NUM_COLS][NUM_COLS]; /* allowed next column indices*/

static bool init_trans = false;

/* initialise column patterns and transition table once */
static void init_transmat(void)
{
    if (init_trans) return;
    /* generate all 5‑bit column patterns that avoid vertical patterns
       001 and 010 */
    int cnt = 0;
    for (int p = 0; p < 32; ++p) {
        bool ok = true;
        for (int i = 0; i < 3; ++i) {
            int b0 = (p >> i) & 1;
            int b1 = (p >> (i + 1)) & 1;
            int b2 = (p >> (i + 2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 0 && b1 == 1 && b2 == 0)) {
                ok = false;
                break;
            }
        }
        if (ok) cols[cnt++] = (uint8_t)p;
    }
    /* cnt must be 14 */
    (void)cnt; /* silence unused variable warning if any */

    /* pre‑compute transition lists */
    for (int a = 0; a < NUM_COLS; ++a) {
        for (int b = 0; b < NUM_COLS; ++b) {
            idx_t id = (idx_t)a * NUM_COLS + b;
            int k = 0;
            for (int c = 0; c < NUM_COLS; ++c) {
                if (horiz_ok(cols[a], cols[b], cols[c])) {
                    next_list[id][k++] = c;
                }
            }
            next_cnt[id] = k;
        }
    }
    init_trans = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined below offset but safe */
    if (n == 1) return (seq_t)NUM_COLS;   /* 14 */

    init_transmat();

    const idx_t states = NUM_COLS * NUM_COLS;
    seq_t dp[states];
    seq_t newdp[states];

    /* base: all pairs for n = 2 are valid */
    for (idx_t i = 0; i < states; ++i) dp[i] = (seq_t)1;

    /* length grows from 2 up to n */
    for (idx_t pos = 3; pos <= n; ++pos) {
        /* zero newdp */
        for (idx_t i = 0; i < states; ++i) newdp[i] = (seq_t)0;

        for (idx_t s = 0; s < states; ++s) {
            seq_t cur = dp[s];
            if (cur == (seq_t)0) continue;
            int k = next_cnt[s];
            for (int t = 0; t < k; ++t) {
                idx_t nxt = next_list[s][t];
                idx_t dst = ((s % NUM_COLS) * NUM_COLS) + nxt;
                newdp[dst] += cur;
            }
        }
        /* swap dp and newdp */
        for (idx_t i = 0; i < states; ++i) dp[i] = newdp[i];
    }

    /* sum all counts for the last two columns */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < states; ++i) result += dp[i];
    return result;
}
