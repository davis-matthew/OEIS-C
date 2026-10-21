/*
 * A207443  Number of 3Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A207443 Number of 3×n 0..1 arrays avoiding 0 0 1 and 0
 *            1 0 horizontally and 0 0 1 and 1 0 0 vertically
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Local constants */
static const int ALLOWED[6] = {0, 2, 3, 5, 6, 7};          /* 000,010,011,101,110,111 */

/* Transition data */
static int  N_STATES;                 /* 36 */
static int  next_cnt[36];
static int  next_state[36][6];        /* up to 6 outgoing transitions */

/* Runtime memoisation */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static idx_t  cache_n    = 0;      /* maximum n computed so far */

static seq_t  state_curr[36];
static seq_t  state_next[36];

/* Helper to initialise transition tables */
static bool init_done = false;
static void init_tables(void)
{
    if (init_done) return;
    init_done = true;
    N_STATES = 36;
    /* Build mapping from (a,b) pair to state index */
    int index_map[8][8]; /* we only use indices of allowed columns, others stay -1 */
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) index_map[i][j] = -1;

    for (int ai = 0; ai < 6; ++ai)
        for (int bi = 0; bi < 6; ++bi)
            index_map[ALLOWED[ai]][ALLOWED[bi]] = ai * 6 + bi;

    /* Precompute transitions */
    for (int ai = 0; ai < 6; ++ai) {
        int a = ALLOWED[ai];
        for (int bi = 0; bi < 6; ++bi) {
            int b = ALLOWED[bi];
            int s = ai * 6 + bi;
            int cnt = 0;
            for (int ci = 0; ci < 6; ++ci) {
                int c = ALLOWED[ci];
                bool bad = false;
                for (int r = 0; r < 3; ++r) {
                    int bit_a = (a >> (2 - r)) & 1;
                    int bit_b = (b >> (2 - r)) & 1;
                    int bit_c = (c >> (2 - r)) & 1;
                    /* Check forbidden patterns 001 and 010 */
                    if (bit_a == 0 && bit_b == 0 && bit_c == 1) { bad = true; break; }
                    if (bit_a == 0 && bit_b == 1 && bit_c == 0) { bad = true; break; }
                }
                if (!bad) {
                    next_state[s][cnt++] = bi * 6 + ci;  /* state (b,c) */
                }
            }
            next_cnt[s] = cnt;
        }
    }

    /* Allocate cache array at start with capacity 2 (for n=1,2) */
    cache_cap = 4;
    cache = (seq_t*) malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);
    cache[0] = 0; /* unused */
    cache[1] = 6; /* n = 1 */
    cache[2] = 36; /* n = 2 */
    cache_n = 2;
    /* Initialize state distribution for n = 2 (all ones) */
    for (int s = 0; s < N_STATES; ++s) state_curr[s] = 1;
}

/* Main function required */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe */
    init_tables();

    /* Grow cache if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute up to n if not yet done */
    if (n > cache_n) {
        for (idx_t len = cache_n + 1; len <= n; ++len) {
            if (len == 1) {
                /* already set in init */
            } else if (len == 2) {
                /* already set in init */
            } else { /* len >= 3 */
                /* Compute next state distribution */
                for (int s = 0; s < N_STATES; ++s) state_next[s] = 0;
                for (int s = 0; s < N_STATES; ++s) {
                    seq_t ways = state_curr[s];
                    if (!ways) continue;
                    int cnt = next_cnt[s];
                    for (int t_idx = 0; t_idx < cnt; ++t_idx) {
                        int t = next_state[s][t_idx];
                        state_next[t] += ways;
                    }
                }
                /* Sum to get a(len) */
                seq_t sum = 0;
                for (int s = 0; s < N_STATES; ++s) sum += state_next[s];
                cache[len] = sum;
                /* Prepare for next iteration */
                for (int s = 0; s < N_STATES; ++s) state_curr[s] = state_next[s];
            }
        }
        cache_n = n;
    }
    return cache[n];
}
