/*
 * A206887  Number of nX6 0..1 arrays avoiding 0 1 0 horizontally and 1 0 0
 *          vertically.
 * https://oeis.org/A206887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A206887
    Number of n×6 0..1 arrays avoiding 0 1 0 horizontally and 1 0 0 vertically.
    Recurrence is computed by dynamic programming on sliding windows of
    two rows.
    - a(1) = 37
    - a(2) = 37^2 = 1369
*/

#include <stdint.h>
#include <stdlib.h>

/* ---------- static state ---------- */
static int init_done = 0;

/* allowed rows (no horizontal 010) */
static uint8_t  allow_row[64];          /* 1 if row is allowed */
static uint8_t  rows[64];               /* indices of allowed rows */
static int      cnt_rows;              /* number of allowed rows */

/* adjacency lists: for state (i,j) -> list of next state indices */
static uint16_t next_start[4096];       /* start index in next_flat   */
static uint8_t  next_len[4096];         /* number of outgoing edges  */
static uint16_t next_flat[4096*37];     /* flattened adjacency lists */

/* dp arrays: use 4096 slots (max 64*64) */
static seq_t dp[4096];
static seq_t newdp[4096];

/* cached sequence values up to cache_sz-1 */
static seq_t *a_cache = NULL;
static idx_t cache_sz = 0;   /* number of allocated slots (must be >= 1) */
static idx_t cache_nmax = 0; /* maximum n for which a_cache[n] is valid */

/* ---------- helper: initialize all static data ---------- */
static void init_state(void)
{
    /* 1. Collect allowed rows (no horizontal 010) */
    cnt_rows = 0;
    for (int r = 0; r < 64; ++r) {
        int ok = 1;
        for (int i = 0; i < 4; ++i) {          /* 4 overlapping triples in 6 bits */
            int tri = (r >> i) & 7;            /* bits i..i+2 */
            if (tri == 0b010) {                /* forbidden pattern 010 */
                ok = 0;
                break;
            }
        }
        allow_row[r] = ok ? 1 : 0;
        if (ok)
            rows[cnt_rows++] = (uint8_t)r;
    }

    /* 2. Build adjacency lists for states (prev_prev, prev) -> next */
    int pos = 0;
    for (int i = 0; i < cnt_rows; ++i) {
        for (int j = 0; j < cnt_rows; ++j) {
            int state = i * cnt_rows + j;     /* unique index < 4096 */
            next_start[state] = (uint16_t)pos;
            next_len[state]   = 0;
            uint8_t a = rows[i];
            uint8_t b = rows[j];
            for (int k = 0; k < cnt_rows; ++k) {
                uint8_t c = rows[k];
                /* vertical 100 forbidden: (a & (~b) & (~c)) == 0 */
                if ( ( (uint8_t)(a & (~b) & (~c)) ) == 0 ) {
                    uint16_t ns = (uint16_t)(j * cnt_rows + k); /* new state */
                    next_flat[pos++] = ns;
                    next_len[state] ++;
                }
            }
        }
    }

    /* 3. Prepare DP arrays for n = 2 */
    int state_cnt = cnt_rows * cnt_rows;
    for (int s = 0; s < state_cnt; ++s)
        dp[s] = (seq_t)1;     /* each allowed pair is valid */

    /* 4. Allocate cache for sequence values up to 16 (will grow dynamically) */
    cache_sz = 16;
    a_cache = (seq_t *)malloc(sizeof(seq_t) * cache_sz);
    if (!a_cache) { /* fall back: use seq_t default ctor? */
        abort();
    }
    a_cache[0] = (seq_t)0;     /* unused; sequence is 1-indexed */
    a_cache[1] = (seq_t)cnt_rows;                     /* a(1) */
    a_cache[2] = (seq_t)cnt_rows * (seq_t)cnt_rows;    /* a(2) */
    cache_nmax = 2;

    init_done = 1;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!init_done)
        init_state();

    if (n < 1)
        return (seq_t)0;               /* undefined, but return 0 */

    /* If requested n is already cached, return it */
    if (n <= cache_nmax)
        return a_cache[n];

    /* Ensure cache large enough */
    if (n + 1 > cache_sz) {
        idx_t new_sz = cache_sz * 2;
        if (new_sz < n + 1)
            new_sz = n + 1;
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_sz);
        cache_sz = new_sz;
    }

    int state_cnt = cnt_rows * cnt_rows;

    /* current dp corresponds to length current_len */
    idx_t current_len = cache_nmax;    /* we have values up to this n */
    /* dp already holds counts for length 2 if current_len==2 */
    /* otherwise dp holds counts for length current_len */

    /* Grow cache and compute up to requested n */
    for (idx_t len = current_len + 1; len <= n; ++len) {
        /* reset newdp */
        for (int s = 0; s < state_cnt; ++s)
            newdp[s] = (seq_t)0;

        /* transition */
        for (int s = 0; s < state_cnt; ++s) {
            seq_t ways = dp[s];
            if (!ways)
                continue;
            int start = next_start[s];
            int cnt   = next_len[s];
            for (int k = 0; k < cnt; ++k) {
                uint16_t ns = next_flat[start + k];
                newdp[ns] += ways;
            }
        }

        /* sum all newdp entries to get a(len) */
        seq_t sum = (seq_t)0;
        for (int s = 0; s < state_cnt; ++s)
            sum += newdp[s];

        /* store */
        a_cache[len] = sum;

        /* swap dp and newdp for next iteration */
        for (int s = 0; s < state_cnt; ++s)
            dp[s] = newdp[s];
    }

    cache_nmax = n;
    return a_cache[n];
}
