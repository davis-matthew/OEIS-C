/*
 * A232954  Number of nX7 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally, diagonally or antidiagonally, and top left
 *          element zero.
 * https://oeis.org/A232954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A232954: number of n×7 arrays over {0,1,2,3} with top-left element
 * zero and no element adjacent (horizontally, diagonally or antidiagonally)
 * to its value complement 3–x.  Computed by dynamic programming over all
 * valid row states (no horizontal complementary pair) and their admissible
 * transitions.  The DP gives the number of arrays with the required
 * adjacency constraints for each n.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------------------- */
/* Global data structures ---------------------------------------------------- */
static int Nstates;                 /* number of valid row states (≈2700) */
static uint8_t *state_digits;       /* [Nstates][7] flattened digits 0..3 */
static int  *adj_start;             /* prefix sums of adjacency lists (size Nstates+1) */
static uint16_t *edges;             /* flattened adjacency lists */
static seq_t *dp_cur, *dp_next;     /* current and next DP vectors */

static seq_t *cache = NULL;         /* cache of a(1..max_cached) */
static idx_t  cache_cap = 0;        /* capacity of cache array */
static idx_t  max_cached = 0;       /* highest n for which cache is populated */

static bool initialized = false;    /* flag for lazy construction of data */

/* --------------------------------------------------------------------------- */
/* Grow cache if needed */
static void grow_cache(idx_t needed)
{
    if (needed < cache_cap) return;
    idx_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------------- */
/* Build all states, adjacency and initial DP vector */
static void build(void)
{
    const int TOTAL_CODES = 4 * 4 * 4 * 4 * 4 * 4 * 4;   /* 4^7 = 16384 */

    /* Enumerate all 4^7 codes and keep those with no horizontal complementary pair */
    int *valid_codes = (int *)malloc(TOTAL_CODES * sizeof(int));
    int valcnt = 0;
    for (int code = 0; code < TOTAL_CODES; ++code) {
        int tmp = code;
        uint8_t d[7];
        for (int j = 0; j < 7; ++j) { d[j] = tmp & 3; tmp >>= 2; }
        int ok = 1;
        for (int j = 0; j < 6; ++j) {
            if (d[j] + d[j + 1] == 3) { ok = 0; break; }
        }
        if (ok) valid_codes[valcnt++] = code;
    }

    Nstates = valcnt;
    state_digits = (uint8_t *)calloc(Nstates * 7, sizeof(uint8_t));
    int *edge_count = (int *)calloc(Nstates, sizeof(int));
    int init_states = 0;

    /* Decode digits for each valid state; count initial states (top‑left == 0) */
    for (int i = 0; i < Nstates; ++i) {
        int code = valid_codes[i];
        int tmp = code;
        for (int j = 0; j < 7; ++j) {
            state_digits[i * 7 + j] = tmp & 3;
            tmp >>= 2;
        }
        if (state_digits[i * 7] == 0) ++init_states;
    }

    /* Count edges for allocation */
    for (int i = 0; i < Nstates; ++i) {
        uint8_t *d1 = state_digits + i * 7;
        for (int j = i; j < Nstates; ++j) {
            uint8_t *d2 = state_digits + j * 7;
            int ok = 1;
            for (int col = 0; col < 7; ++col) {
                if (col > 0   && d1[col] + d2[col - 1] == 3) { ok = 0; break; }
                if (col < 6   && d1[col] + d2[col + 1] == 3) { ok = 0; break; }
            }
            if (ok) {
                ++edge_count[i];
                if (i != j) ++edge_count[j];
            }
        }
    }

    /* Build adjacency lists */
    adj_start = (int *)malloc((Nstates + 1) * sizeof(int));
    adj_start[0] = 0;
    for (int i = 0; i < Nstates; ++i) adj_start[i + 1] = adj_start[i] + edge_count[i];
    int total_edges = adj_start[Nstates];
    edges = (uint16_t *)malloc(total_edges * sizeof(uint16_t));

    int *pos = (int *)malloc(Nstates * sizeof(int));
    memcpy(pos, adj_start, Nstates * sizeof(int));
    for (int i = 0; i < Nstates; ++i) {
        uint8_t *d1 = state_digits + i * 7;
        for (int j = i; j < Nstates; ++j) {
            uint8_t *d2 = state_digits + j * 7;
            int ok = 1;
            for (int col = 0; col < 7; ++col) {
                if (col > 0   && d1[col] + d2[col - 1] == 3) { ok = 0; break; }
                if (col < 6   && d1[col] + d2[col + 1] == 3) { ok = 0; break; }
            }
            if (ok) {
                edges[pos[i]++] = (uint16_t)j;
                if (i != j) edges[pos[j]++] = (uint16_t)i;
            }
        }
    }
    free(pos);
    free(edge_count);
    free(valid_codes);

    /* Allocate DP vectors */
    dp_cur  = (seq_t *)calloc(Nstates, sizeof(seq_t));
    dp_next = (seq_t *)calloc(Nstates, sizeof(seq_t));

    /* Initialise DP for the first row */
    for (int i = 0; i < Nstates; ++i)
        if (state_digits[i * 7] == 0) dp_cur[i] = (seq_t)1;

    /* Compute a(1) */
    seq_t total = 0;
    for (int i = 0; i < Nstates; ++i) total += dp_cur[i];

    /* Prepare cache */
    cache_cap = init_states + 1;          /* at least one entry for n=1 */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[1] = total;
    max_cached = 1;
}

/* --------------------------------------------------------------------------- */
/* Public sequence routine */
seq_t a(idx_t n)
{
    if (!initialized) { build(); initialized = true; }

    if (n <= max_cached) return cache[n];

    /* Extend DP to desired n */
    for (idx_t step = max_cached + 1; step <= n; ++step) {
        /* Transition dp_cur -> dp_next */
        memset(dp_next, 0, Nstates * sizeof(seq_t));
        for (int i = 0; i < Nstates; ++i) {
            seq_t val = dp_cur[i];
            if (!val) continue;
            int beg = adj_start[i];
            int end = adj_start[i + 1];
            for (int e = beg; e < end; ++e) {
                dp_next[edges[e]] += val;
            }
        }
        /* Swap vectors */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* Sum current distributions to obtain a(step) */
        seq_t sum = 0;
        for (int i = 0; i < Nstates; ++i) sum += dp_cur[i];

        /* Store result */
        grow_cache(step);
        cache[step] = sum;
    }

    max_cached = n;
    return cache[n];
}
