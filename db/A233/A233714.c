/*
 * A233714  Number of (n+1) X (5+1) 0..4 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 14.
 * https://oeis.org/A233714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of (n+1)×6 arrays of digits 0..4
 * for which every 2×2 subblock has the sum of the squares of
 * the four edge differences equal to 14.
 *
 * The sequence is computed by dynamic programming over rows.
 * States are all 5^6=15625 possible rows; for each state we store
 * all compatible next rows.
 * The implementation is fully cache‑aware, memoises results for
 * arbitrary calling order, and works with any build‑time
 * definition of seq_t (unsigned integer or arbitrary‑precision
 * class).
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* constant: number of 5‑ary digits per row */
#define STATES 15625            /* 5^6 */
#define MAX_PAIRS 25           /* maximum number of (u,v) pairs for a given (a,b) */

static uint8_t pair_u[5][5][MAX_PAIRS];
static uint8_t pair_v[5][5][MAX_PAIRS];
static uint8_t pair_cnt[5][5];

static uint32_t *offsets = NULL;     /* offsets[s] = start index in neighbor_pool */
static uint16_t *neighbor_pool = NULL; /* concatenated neighbor lists */

/* DP buffers */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* memoised sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* highest n for which cache[n] is valid */

/* one‑time initialisation flag */
static bool initialized = false;

/* -------------------------------------------------------------------------- */
/* Decode state number into base‑5 digits 0..4 (most significant first) */
static void decode_state(uint32_t state, uint8_t d[6])
{
    int i;
    for (i = 5; i >= 0; --i) {
        d[i] = state % 5;
        state /= 5;
    }
}

/* -------------------------------------------------------------------------- */
/* Build tables of allowed (u,v) pairs for each (a,b) */
static void build_pair_tables(void)
{
    int a, b, u, v;
    for (a = 0; a < 5; ++a)
        for (b = 0; b < 5; ++b) {
            uint8_t cnt = 0;
            int d_ab = a - b;
            int sq_ab = d_ab * d_ab;
            for (u = 0; u < 5; ++u)
                for (v = 0; v < 5; ++v) {
                    int sum =
                        sq_ab +
                        (a - u) * (a - u) +
                        (b - v) * (b - v) +
                        (u - v) * (u - v);
                    if (sum == 14) {
                        pair_u[a][b][cnt] = (uint8_t)u;
                        pair_v[a][b][cnt] = (uint8_t)v;
                        ++cnt;
                    }
                }
            pair_cnt[a][b] = cnt;
        }
}

/* -------------------------------------------------------------------------- */
/* Recursive helper to count compatible neighbor rows */
static int recurse_count(int pos, const uint8_t d[6], uint8_t t[6])
{
    if (pos == 5)          /* all 6 digits set */
        return 1;
    int total = 0;
    uint8_t a = d[pos];
    uint8_t b = d[pos + 1];
    uint8_t need = t[pos];
    uint8_t curcnt = pair_cnt[a][b];
    for (int k = 0; k < curcnt; ++k) {
        if (pair_u[a][b][k] == need) {
            t[pos + 1] = pair_v[a][b][k];
            total += recurse_count(pos + 1, d, t);
        }
    }
    return total;
}

/* Count number of neighbors for a given state */
static int count_neighbors(const uint8_t d[6])
{
    uint8_t t[6];
    int total = 0;
    uint8_t a0 = d[0];
    uint8_t b0 = d[1];
    uint8_t curcnt = pair_cnt[a0][b0];
    for (int k = 0; k < curcnt; ++k) {
        t[0] = pair_u[a0][b0][k];
        t[1] = pair_v[a0][b0][k];
        total += recurse_count(1, d, t);
    }
    return total;
}

/* -------------------------------------------------------------------------- */
/* Recursive helper to store all neighbor rows in pool */
static void recurse_fill(int pos, const uint8_t d[6], uint8_t t[6],
                         uint16_t *code_out, size_t *idx)
{
    if (pos == 5) {
        uint32_t code = 0;
        for (int i = 0; i < 6; ++i)
            code = code * 5 + t[i];
        code_out[*idx] = (uint16_t)code;
        ++(*idx);
        return;
    }
    uint8_t a = d[pos];
    uint8_t b = d[pos + 1];
    uint8_t need = t[pos];
    uint8_t curcnt = pair_cnt[a][b];
    for (int k = 0; k < curcnt; ++k) {
        if (pair_u[a][b][k] == need) {
            t[pos + 1] = pair_v[a][b][k];
            recurse_fill(pos + 1, d, t, code_out, idx);
        }
    }
}

/* Fill neighbors for a given state, starting at start_index in pool */
static void fill_neighbors(uint32_t state, size_t start_index)
{
    uint8_t d[6];
    decode_state(state, d);
    uint8_t t[6];
    size_t idx = start_index;
    uint8_t a0 = d[0];
    uint8_t b0 = d[1];
    uint8_t curcnt = pair_cnt[a0][b0];
    for (int k = 0; k < curcnt; ++k) {
        t[0] = pair_u[a0][b0][k];
        t[1] = pair_v[a0][b0][k];
        recurse_fill(1, d, t, neighbor_pool, &idx);
    }
    /* idx should equal start_index + deg[state] */
}

/* -------------------------------------------------------------------------- */
/* Build adjacency structure once */
static void build_adjacency(void)
{
    static uint32_t deg[STATES];
    size_t i;
    size_t total_edges = 0;

    /* first pass: compute degree of each state */
    for (i = 0; i < STATES; ++i) {
        uint8_t d[6];
        decode_state((uint32_t)i, d);
        deg[i] = count_neighbors(d);
        total_edges += deg[i];
    }

    /* allocate offsets and pool */
    offsets = (uint32_t *)malloc((STATES + 1) * sizeof(uint32_t));
    neighbor_pool = (uint16_t *)malloc(total_edges * sizeof(uint16_t));

    offsets[0] = 0;
    for (i = 0; i < STATES; ++i)
        offsets[i + 1] = offsets[i] + deg[i];

    /* second pass: fill neighbor lists */
    for (i = 0; i < STATES; ++i)
        fill_neighbors((uint32_t)i, offsets[i]);

    /* free temporary deg array is static */
}

/* -------------------------------------------------------------------------- */
/* One-time initialisation */
static void init(void)
{
    if (initialized)
        return;
    initialized = true;

    build_pair_tables();
    build_adjacency();

    dp_cur = (seq_t *)malloc(STATES * sizeof(seq_t));
    dp_next = (seq_t *)malloc(STATES * sizeof(seq_t));

    /* initial row vector: one way for each state */
    for (size_t i = 0; i < STATES; ++i)
        dp_cur[i] = (seq_t)1;

    /* initialise cache */
    cache_cap = 1;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)0;   /* unused placeholder */

    cache_len = 0;         /* no terms computed yet */
}

/* -------------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* sequence starts at offset 1 */
        return (seq_t)0;

    init();

    /* extend memoised values up to n */
    while (cache_len < (size_t)n) {
        /* clear next DP array */
        memset(dp_next, 0, STATES * sizeof(seq_t));
        seq_t step_sum = (seq_t)0;

        /* propagate counts */
        for (size_t s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0)
                continue;
            uint32_t start = offsets[s];
            uint32_t end   = offsets[s + 1];
            for (uint32_t idx = start; idx < end; ++idx) {
                uint16_t t = neighbor_pool[idx];
                dp_next[t] += val;
            }
        }

        /* compute total for this step: a(cache_len+1) */
        for (size_t i = 0; i < STATES; ++i)
            step_sum += dp_next[i];

        /* store in cache */
        ++cache_len;                          /* cache_len == cache index to store */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = step_sum;

        /* swap DP buffers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return cache[(size_t)n];
}
