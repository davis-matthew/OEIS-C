/*
 * A233789  Number of (n+1)X(5+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 11.
 * https://oeis.org/A233789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of (n+1)×6 arrays over {0,1,2,3} in which
 * every 2×2 subblock has the sum of squares of all six edge and
 * diagonal differences equal to 11.
 *
 * This implementation constructs the graph of all 4096 possible
 * 6‑digit rows (base‑4 representation).  Two rows are adjacent
 * if the five overlapping 2×2 blocks between them satisfy the
 * stated condition.  A directed walk of length n in this graph
 * corresponds to one admissible (n+1)×6 array; thus a(n) is the
 * total number of such walks, which we obtain by dynamic programming.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* constants */
#define ROWS      4096      /* 4^6 rows */
#define MAX_DEG   512       /* generous upper bound on a row’s degree */

/* global tables */
static uint8_t  row_digits[ROWS][6];          /* base‑4 digits of a row */
static bool     quad_valid[256];             /* 4^4 = 256 possible 2×2 blocks */
static uint16_t adjacency[ROWS * MAX_DEG];   /* neighbours stored densely */
static uint8_t  adjacency_deg[ROWS];         /* number of neighbours per row */

/* DP working arrays */
static seq_t dp_cur[ROWS];
static seq_t dp_next[ROWS];

/* cache for already computed a(n) values */
static seq_t *seq_cache = NULL;
static idx_t  cache_cap = 0;   /* maximum index stored (>= n) */
static idx_t  cache_len = 0;   /* highest n computed so far */

/* flag to ensure one‑time initialization */
static bool initialized = false;

/* --------------------------------------------------------------------- */
static void init_tables(void)
{
    /* 1. precompute row digits (base‑4 representation) */
    for (int i = 0; i < ROWS; ++i) {
        uint16_t t = (uint16_t)i;
        for (int d = 0; d < 6; ++d) {
            row_digits[i][d] = (uint8_t)(t & 3);
            t >>= 2;
        }
    }

    /* 2. precompute validity of all 4×4 = 256 2×2 blocks */
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c)
                for (int d = 0; d < 4; ++d) {
                    int da = a - b;
                    int db = c - d;
                    int dc = a - c;
                    int dd = b - d;
                    int de = a - d;
                    int df = b - c;
                    int sumsq = da * da + db * db + dc * dc + dd * dd
                                + de * de + df * df;
                    int idx = ((((a << 2) + b) << 2) + c) * 4 + d;
                    quad_valid[idx] = (sumsq == 11);
                }

    /* 3. build adjacency lists */
    memset(adjacency_deg, 0, sizeof(adjacency_deg));

    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = r1 + 1; r2 < ROWS; ++r2) {
            bool ok = true;
            for (int col = 0; col < 5; ++col) {
                int a = row_digits[r1][col];
                int b = row_digits[r1][col + 1];
                int c = row_digits[r2][col];
                int dval = row_digits[r2][col + 1];
                int idx = ((((a << 2) + b) << 2) + c) * 4 + dval;
                if (!quad_valid[idx]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                int pos1 = r1 * MAX_DEG + adjacency_deg[r1]++;
                adjacency[pos1] = (uint16_t)r2;
                int pos2 = r2 * MAX_DEG + adjacency_deg[r2]++;
                adjacency[pos2] = (uint16_t)r1;
            }
        }
    }

    /* 4. initialise DP for step 0: one way to have a single row */
    for (int i = 0; i < ROWS; ++i)
        dp_cur[i] = (seq_t)1;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* offset starts at 1 */

    /* lazy init */
    if (!initialized) {
        init_tables();
        cache_cap = 64;
        seq_cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        cache_len  = 0;
        initialized = true;
    }

    /* compute values up to requested n */
    while (cache_len < n) {
        idx_t step = cache_len + 1;   /* compute a(step) */

        /* zero dp_next */
        memset(dp_next, 0, sizeof(dp_next));

        /* transition */
        for (int r = 0; r < ROWS; ++r) {
            seq_t val = dp_cur[r];
            if (val == (seq_t)0)
                continue;
            int deg = adjacency_deg[r];
            for (int k = 0; k < deg; ++k) {
                int neigh = adjacency[r * MAX_DEG + k];
                dp_next[neigh] += val;
            }
        }

        /* sum to get a(step) */
        seq_t total = (seq_t)0;
        for (int r = 0; r < ROWS; ++r)
            total += dp_next[r];

        /* resize cache if needed */
        if (step > cache_cap) {
            idx_t new_cap = cache_cap * 2;
            while (new_cap < step)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(seq_cache,
                                          (new_cap + 1) * sizeof(seq_t));
            if (!tmp) {
                /* out of memory – return computed value anyway */
                return total;
            }
            seq_cache = tmp;
            cache_cap = new_cap;
        }
        seq_cache[step] = total;

        /* prepare for next step: dp_cur = dp_next, dp_next cleared next loop */
        memcpy(dp_cur, dp_next, sizeof(dp_next));
        ++cache_len;
    }

    return seq_cache[n];
}
