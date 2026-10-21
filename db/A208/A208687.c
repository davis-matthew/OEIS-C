/*
 * A208687  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A208687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   pat_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) = number of n×7 binary arrays avoiding
 *  horizontal triples 000 and 001, and vertical triples 011 and 101.
 *  The algorithm uses a transfer‑matrix over the last two rows.
 *  Horizontal validity is pre‑computed for all 7‑bit patterns.
 *  Each state is a pair (row_i,row_j) of horizontal‑valid rows.
 *  A transition to a new row row_k is allowed iff for every
 *  column the 3‑tuple (row_i,row_j,row_k) is not 011 or 101.
 *  The DP iterates from n=2 up to the requested n, accumulating
 *  the number of arrays for each state. The final answer is the sum
 *  over all states after processing n rows.
 *
 *  Complexity:  O(n·42²) arithmetic operations.
 *  For n≤10 000 it works in well under 2 s for both 64‑bit and
 *  arbitrary‑precision seq_t.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/pat_ok.h"

static uint8_t H[42];                 /* horizontal‑valid rows */
static int8_t row_to_idx[128];        /* mapping from pattern to index in H or -1 */
static unsigned char adj_cnt[42*42];  /* number of transitions from each state */
static unsigned short adj_to[42*42*42];  /* list of target states */
static unsigned int base_idx[42*42];  /* starting offset into adj_to for each state */
static int init_done = 0;

static inline int vertical_ok(uint8_t a, uint8_t b, uint8_t c) {
    /* check vertical patterns 011 and 101 */
    for (int col = 0; col < 7; ++col) {
        int x = (a >> col) & 1;
        int y = (b >> col) & 1;
        int z = (c >> col) & 1;
        if (x == 0 && y == 1 && z == 1) return 0;
        if (x == 1 && y == 0 && z == 1) return 0;
    }
    return 1;
}

static void init_tables(void) {
    int hcnt = 0;
    for (int pat = 0; pat < 128; ++pat) {
        row_to_idx[pat] = -1;
    }
    for (int pat = 0; pat < 128; ++pat) {
        if (pat_ok((uint8_t)pat)) {
            H[hcnt] = (uint8_t)pat;
            row_to_idx[pat] = (int8_t)hcnt;
            ++hcnt;
        }
    }
    /* hcnt must be 42 */
    int states = hcnt * hcnt;          /* 42 * 42 = 1764 */
    int off = 0;
    for (int i = 0; i < hcnt; ++i) {
        for (int j = 0; j < hcnt; ++j) {
            int src = i * hcnt + j;
            base_idx[src] = off;
            unsigned int cnt = 0;
            for (int k = 0; k < hcnt; ++k) {
                if (vertical_ok(H[i], H[j], H[k])) {
                    adj_to[off++] = (unsigned short)(j * hcnt + k);
                    ++cnt;
                }
            }
            adj_cnt[src] = (unsigned char)cnt;
        }
    }
    init_done = 1;
}

seq_t a(idx_t n) {
    /* +--- static arrays for DP (one row per state) */
    static seq_t cur[42 * 42];
    static seq_t nxt[42 * 42];
    if (!init_done) init_tables();

    if (n == 1) return (seq_t)42;          /* a(1) */
    if (n == 2) return (seq_t)1764;         /* 42^2 */

    int states = 42 * 42;                  /* 1764 */
    /* initialise each state with 1 (all pairs of horizontal rows are allowed) */
    for (int i = 0; i < states; ++i) cur[i] = (seq_t)1;

    seq_t *cur_ptr = cur;
    seq_t *next_ptr = nxt;
    for (idx_t step = 3; step <= n; ++step) {
        /* zero next array */
        for (int i = 0; i < states; ++i) next_ptr[i] = (seq_t)0;

        for (int src = 0; src < states; ++src) {
            seq_t val = cur_ptr[src];
            if (!val) continue;
            int start = base_idx[src];
            int end   = start + adj_cnt[src];
            for (int p = start; p < end; ++p) {
                int tgt = adj_to[p];
                next_ptr[tgt] += val;
            }
        }
        /* swap pointers */
        seq_t *tmp = cur_ptr;
        cur_ptr = next_ptr;
        next_ptr = tmp;
    }

    /* sum over all states to obtain total number of arrays */
    seq_t total = (seq_t)0;
    for (int i = 0; i < states; ++i) total += cur_ptr[i];
    return total;
}
