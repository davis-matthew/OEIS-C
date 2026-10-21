/*
 * A233871  Number of (n+1) X (3+1) 0..7 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 21.
 * https://oeis.org/A233871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   absdiff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A233871 via dynamic programming over rows. The
   number of (n+1)×4 arrays over {0,…,7} whose every 2×2 subblock
   satisfies the sum‑of‑abs‑differences‑is‑21 condition. */

/* The row space has 8⁴ = 4096 possibilities. */
#define ROWS 4096

#include <stdbool.h>
#include <stdint.h>
#include "../utils/absdiff.h"

/* adjacency lists: for each row 's', neighbors[s] holds all rows 't'
   that are compatible with 's'.  The first neighbor_count[s] entries
   of the array are valid. */
static uint16_t neighbors[ROWS][ROWS];
static uint16_t neighbor_count[ROWS];
static bool adjacency_built = false;

/* absolute difference of two 0–7 values */

/* check whether rows 's' and 't' can be placed one above the other. */
static bool compatible(uint16_t s, uint16_t t) {
    int s0 =  s        & 7;
    int s1 = (s >>  3) & 7;
    int s2 = (s >>  6) & 7;
    int s3 = (s >>  9) & 7;
    int t0 =  t        & 7;
    int t1 = (t >>  3) & 7;
    int t2 = (t >>  6) & 7;
    int t3 = (t >>  9) & 7;

    int sum;
    /* columns 0–1 */
    sum = absdiff(s0,s1)+absdiff(t0,t1)+absdiff(s0,t0)+absdiff(s1,t1)
          +absdiff(s0,t1)+absdiff(s1,t0);
    if (sum != 21) return false;

    /* columns 1–2 */
    sum = absdiff(s1,s2)+absdiff(t1,t2)+absdiff(s1,t1)+absdiff(s2,t2)
          +absdiff(s1,t2)+absdiff(s2,t1);
    if (sum != 21) return false;

    /* columns 2–3 */
    sum = absdiff(s2,s3)+absdiff(t2,t3)+absdiff(s2,t2)+absdiff(s3,t3)
          +absdiff(s2,t3)+absdiff(s3,t2);
    return sum == 21;
}

/* build all compatibility lists once */
static void build_adjacency(void) {
    if (adjacency_built) return;
    for (uint16_t s = 0; s < ROWS; ++s) {
        uint16_t cnt = 0;
        for (uint16_t t = 0; t < ROWS; ++t)
            if (compatible(s, t))
                neighbors[s][cnt++] = t;
        neighbor_count[s] = cnt;
    }
    adjacency_built = true;
}

/* external function required by the task */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* behaviour below offset is irrelevant */

    build_adjacency();

    static seq_t cur[ROWS];
    static seq_t nxt[ROWS];

    /* start with all possible single rows */
    for (uint16_t i = 0; i < ROWS; ++i) cur[i] = (seq_t)1;

    seq_t result = 0;

    for (idx_t step = 1; step <= n; ++step) {
        /* reset next array */
        for (uint16_t i = 0; i < ROWS; ++i) nxt[i] = (seq_t)0;

        /* transition */
        for (uint16_t s = 0; s < ROWS; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;
            uint16_t *list = neighbors[s];
            uint16_t cnt   = neighbor_count[s];
            for (uint16_t j = 0; j < cnt; ++j)
                nxt[list[j]] += val;
        }

        /* sum of all entries gives a(step) */
        seq_t sum = 0;
        for (uint16_t i = 0; i < ROWS; ++i) sum += nxt[i];

        if (step == n) {
            result = sum;
            break;
        }

        /* prepare for next iteration */
        for (uint16_t i = 0; i < ROWS; ++i) cur[i] = nxt[i];
    }

    return result;
}
