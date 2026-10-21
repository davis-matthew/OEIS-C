/*
 * A201083  Number of -3..3 arrays of n elements with first and second
 *          differences also in -3..3.
 * https://oeis.org/A201083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by a linear‑recurrence obtained from * dynamic
 *            programming on 49 pair‑states (prev, curr) where * prev,curr
 *            ∈{-3,…,3}. The transitions enforce * |curr‑prev| ≤ 3 (first
 *            difference) * |(next‑curr)‑ (curr‑prev)| ≤ 3 (second difference)
 *            * * The recurrence (empirically) is * a(n)=4*a(n‑1...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define OFFSET (-3)
enum { STATE_CNT = 7 * 7 };            /* 49 pair‑states (prev,curr) */

/* map (prev,curr) to 0…48 */
static inline idx_t state_idx(int prev, int curr)
{
    return (idx_t)((prev - OFFSET) * 7 + (curr - OFFSET));
}

/* adjacency for each state */
static idx_t trans_to[STATE_CNT][7];
static unsigned char trans_count[STATE_CNT];
static bool trans_built = false;

/* build all transitions once */
static void build_transitions(void)
{
    if (trans_built) return;
    for (int p = -3; p <= 3; ++p) {
        for (int c = -3; c <= 3; ++c) {
            idx_t s = state_idx(p, c);
            trans_count[s] = 0;
            int d1 = c - p;
            if (d1 < -3 || d1 > 3) continue;      /* not allowed first diff */
            for (int n = -3; n <= 3; ++n) {
                int d2 = n - c;
                if (d2 < -3 || d2 > 3) continue;  /* not allowed first diff of next */
                int sdiff = d2 - d1;              /* second difference */
                if (sdiff < -3 || sdiff > 3) continue;
                trans_to[s][trans_count[s]++] = state_idx(c, n);
            }
        }
    }
    trans_built = true;
}

/* DP arrays (static to avoid re‑allocation on each call) */
static seq_t dp_cur[STATE_CNT];
static seq_t dp_next[STATE_CNT];

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset is 1, value for n=0 is irrelevant */
    if (n == 1) return 7;    /* length‑1 sequences: any single element */

    build_transitions();

    /* initialise for length 2: all valid first‑difference pairs */
    for (int i = 0; i < STATE_CNT; ++i) dp_cur[i] = 0;
    for (int p = -3; p <= 3; ++p) {
        for (int c = -3; c <= 3; ++c) {
            int d = c - p;
            if (d < -3 || d > 3) continue;
            dp_cur[state_idx(p, c)] = 1;
        }
    }

    if (n == 2) {
        seq_t total = 0;
        for (int i = 0; i < STATE_CNT; ++i) total += dp_cur[i];
        return total;
    }

    for (idx_t len = 3; len <= n; ++len) {
        for (int i = 0; i < STATE_CNT; ++i) dp_next[i] = 0;
        for (int s = 0; s < STATE_CNT; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            unsigned char k = trans_count[s];
            for (unsigned char t = 0; t < k; ++t)
                dp_next[trans_to[s][t]] += val;
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < STATE_CNT; ++i) dp_cur[i] = dp_next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < STATE_CNT; ++i) total += dp_cur[i];
    return total;
}
