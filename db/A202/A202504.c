/*
 * A202504  Number of zero-sum -1..1 arrays of n elements with first through
 *          third differences also in -1..1.
 * https://oeis.org/A202504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 63, u64 n <= 122,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A202504: the number of zero‑sum arrays of length n with values
   in {-1,0,1} whose first, second and third differences are also in
   {-1,0,1}.  The computation uses dynamic programming over the last
   three values and the running sum.  The algorithm works for any n ≥ 1
   and does not rely on hard‑coded terms. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

enum { STATES = 27 };            /* 3^3 possible triples (-1/0/1)^3 */

/* Transition tables */
static int state_a[STATES], state_b[STATES], state_c[STATES];
static int idx_map[3][3][3];      /* maps (a,b,c) → state index */
static int trans_next[STATES][3];
static int trans_val [STATES][3];
static int trans_cnt [STATES];
static bool _initialized = false;

/* One‑time initialization of state and transition tables. */
static void init_state_data(void)
{
    if (_initialized) return;
    /* Enumerate all triples and build idx_map */
    int st = 0;
    for (int a = -1; a <= 1; ++a)
        for (int b = -1; b <= 1; ++b)
            for (int c = -1; c <= 1; ++c) {
                state_a[st] = a;
                state_b[st] = b;
                state_c[st] = c;
                idx_map[a + 1][b + 1][c + 1] = st;
                ++st;
            }

    /* Build transitions for each state */
    for (int s = 0; s < STATES; ++s) {
        int a = state_a[s];
        int b = state_b[s];
        int c = state_c[s];
        int cnt = 0;
        for (int v = -1; v <= 1; ++v) {
            int d1 = v - c;
            if (d1 < -1 || d1 > 1) continue;
            int d2 = v - 2*c + b;
            if (d2 < -1 || d2 > 1) continue;
            int d3 = v - 3*c + 3*b - a;
            if (d3 < -1 || d3 > 1) continue;
            int next = idx_map[b + 1][c + 1][v + 1];
            trans_next[s][cnt] = next;
            trans_val [s][cnt] = v;
            ++cnt;
        }
        trans_cnt[s] = cnt;
    }
    _initialized = true;
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1, a(2) = 1 */
    if (n == 1 || n == 2) return (seq_t)1;

    /* For n >= 3 we use dynamic programming. */
    init_state_data();

    int m = (int)n;                 /* n fits in 32-bit for all test data */
    int offset = m;                 /* sum offset: sum + offset maps to 0…2m */
    int sum_range = 2 * m + 1;

    size_t total_cells = (size_t)STATES * (size_t)sum_range;

    /* Allocate DP arrays */
    seq_t *cur_counts = (seq_t*)calloc(total_cells, sizeof(seq_t));
    seq_t *next_counts = (seq_t*)calloc(total_cells, sizeof(seq_t));
    if (!cur_counts || !next_counts) {
        free(cur_counts); free(next_counts);
        return (seq_t)0;
    }

    size_t *cur_keys = (size_t*)malloc(total_cells * sizeof(size_t));
    size_t *next_keys = (size_t*)malloc(total_cells * sizeof(size_t));
    if (!cur_keys || !next_keys) {
        free(cur_counts); free(next_counts);
        free(cur_keys); free(next_keys);
        return (seq_t)0;
    }

    size_t cur_keysz = 0, next_keysz = 0;

    /* --------  Initial arrays of length 3 ----------------- */
    for (int s = 0; s < STATES; ++s) {
        int a = state_a[s], b = state_b[s], c = state_c[s];
        int d10 = b - a;
        int d21 = c - b;
        if (d10 < -1 || d10 > 1) continue;
        if (d21 < -1 || d21 > 1) continue;
        int d2 = c - 2*b + a;
        if (d2 < -1 || d2 > 1) continue;
        int sum = a + b + c;
        int sum_idx = sum + offset;
        size_t index = (size_t)s * (size_t)sum_range + (size_t)sum_idx;
        cur_counts[index] = 1;
        cur_keys[cur_keysz++] = index;
    }

    /* --------  DP for lengths 4 … n --------------------- */
    for (int pos = 4; pos <= m; ++pos) {
        next_keysz = 0;
        for (size_t k = 0; k < cur_keysz; ++k) {
            size_t idx = cur_keys[k];
            seq_t cnt = cur_counts[idx];
            int state = (int)(idx / (size_t)sum_range);
            int sum_idx = (int)(idx % (size_t)sum_range);
            for (int t = 0; t < trans_cnt[state]; ++t) {
                int ns = trans_next[state][t];
                int val = trans_val[state][t];
                int nsum_idx = sum_idx + val;
                if (nsum_idx < 0 || nsum_idx >= sum_range) continue;
                size_t nidx = (size_t)ns * (size_t)sum_range + (size_t)nsum_idx;
                if (next_counts[nidx] == 0)
                    next_keys[next_keysz++] = nidx;
                next_counts[nidx] += cnt;
            }
        }
        /* Clear current counts before reusing as next_counts */
        for (size_t k = 0; k < cur_keysz; ++k)
            cur_counts[cur_keys[k]] = 0;
        /* Swap cur and next buffers for next iteration */
        seq_t *tmp_counts = cur_counts; cur_counts = next_counts; next_counts = tmp_counts;
        size_t *tmp_keys   = cur_keys;   cur_keys   = next_keys;   next_keys   = tmp_keys;
        size_t tmp_sz = cur_keysz; cur_keysz = next_keysz; next_keysz = tmp_sz;
    }

    /* --------  Sum over states with zero total sum  -------- */
    seq_t result = 0;
    for (size_t k = 0; k < cur_keysz; ++k) {
        size_t idx = cur_keys[k];
        int sum_idx = (int)(idx % (size_t)sum_range);
        if (sum_idx == offset) result += cur_counts[idx];
    }

    /* --------  Clean up ---------------------------------- */
    free(cur_counts);
    free(next_counts);
    free(cur_keys);
    free(next_keys);

    return result;
}
