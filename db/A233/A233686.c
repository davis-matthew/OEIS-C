/*
 * A233686  Number of (n+1)X(3+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 10, and no
 *          two adjacent values equal.
 * https://oeis.org/A233686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A233686: number of (n+1)×4 arrays of 0..3 with no equal
 * adjacent values and every 2×2 subblock having sum of squares of the edge
 * differences equal to 10.
 * A DP over admissible row patterns is used; the result is obtained by
 * iterating the adjacency relation between consecutive rows.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_CNT 108            /* number of admissible 4‑element rows */

static uint8_t states[STATE_CNT][4];          /* row contents */
static uint16_t adj[STATE_CNT][STATE_CNT];   /* adjacency list (dense matrix) */
static uint8_t deg[STATE_CNT];               /* outdegree of each state */
static int initialized = 0;

/* Build all admissible row states and the transition graph. */
static void build_states_and_adjacency(void)
{
    int a, b, c, d, idx = 0;

    /* Generate all states with no equal neighbours. */
    for (a = 0; a < 4; ++a)
        for (b = 0; b < 4; ++b)
            if (b != a)
                for (c = 0; c < 4; ++c)
                    if (c != b)
                        for (d = 0; d < 4; ++d)
                            if (d != c) {
                                states[idx][0] = (uint8_t)a;
                                states[idx][1] = (uint8_t)b;
                                states[idx][2] = (uint8_t)c;
                                states[idx][3] = (uint8_t)d;
                                ++idx;
                            }
    /* Initialize degrees. */
    for (int i = 0; i < STATE_CNT; ++i) deg[i] = 0;

    /* Build adjacency: check 2×2 blocks. */
    for (int i = 0; i < STATE_CNT; ++i) {
        for (int j = 0; j < STATE_CNT; ++j) {
            int ok = 1;
            for (int k = 0; k < 3 && ok; ++k) {
                int s1 = states[i][k];
                int s2 = states[i][k + 1];
                int t1 = states[j][k];
                int t2 = states[j][k + 1];
                int d1 = s1 > s2 ? s1 - s2 : s2 - s1;
                int d2 = s1 > t1 ? s1 - t1 : t1 - s1;
                int d3 = s2 > t2 ? s2 - t2 : t2 - s2;
                int d4 = t1 > t2 ? t1 - t2 : t2 - t1;
                int sumsq = d1 * d1 + d2 * d2 + d3 * d3 + d4 * d4;
                if (sumsq != 10) ok = 0;
            }
            if (ok) {
                adj[i][deg[i]] = (uint16_t)j;
                ++deg[i];
            }
        }
    }
    initialized = 1;
}

/* Cache for already computed sequence values. */
static seq_t *cache = NULL;      /* cache[n] = a(n) */
static idx_t cache_size = 0;     /* allocated size */
static idx_t cache_valid_up_to = 0;  /* maximum n computed */

/* DP vectors for the current and next layer. */
static seq_t cur[STATE_CNT];
static seq_t nextv[STATE_CNT];

/* Ensure the cache array is large enough. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;
    idx_t newsize = n + 1;
    seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!newcache) exit(EXIT_FAILURE);
    cache = newcache;
    cache_size = newsize;
}

/* Return the nth term of A233686 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Build state transition graph on first use. */
    if (!initialized) build_states_and_adjacency();

    ensure_cache(n);

    if (n <= cache_valid_up_to) return cache[n];

    /* Initialise DP vectors once. */
    if (cache_valid_up_to == 0) {
        for (int i = 0; i < STATE_CNT; ++i) cur[i] = (seq_t)1;
        cache[0] = (seq_t)0;   /* dummy, not used */
    }

    /* Compute values up to n. */
    for (idx_t step = cache_valid_up_to + 1; step <= n; ++step) {
        memset(nextv, 0, sizeof(nextv));
        for (int i = 0; i < STATE_CNT; ++i) {
            seq_t val = cur[i];
            if (val == (seq_t)0) continue;
            for (uint8_t d = 0; d < deg[i]; ++d) {
                int j = adj[i][d];
                nextv[j] = nextv[j] + val;
            }
        }

        /* Sum of nextv gives a(step). */
        seq_t total = (seq_t)0;
        for (int j = 0; j < STATE_CNT; ++j) total = total + nextv[j];
        cache[step] = total;

        memcpy(cur, nextv, sizeof(cur));
    }

    cache_valid_up_to = n;
    return cache[n];
}
