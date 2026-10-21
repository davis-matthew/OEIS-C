/*
 * A230836  Number of n X 3 0..3 arrays x(i,j) with each element horizontally,
 *          vertically or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, and upper left element zero.
 * https://oeis.org/A230836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of n×3 arrays of 0..3 with upper‑left element 0
    such that every entry has a horizontally, vertically or
    antidiagonally adjacent entry equal to (value+1) mod 4.
    The function uses dynamic programming over rows and memoises
    computed terms. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build; do not redeclare them. */

static int val[64][3];                /* base‑4 digits of code */
static int inc[4] = {1, 2, 3, 0};     /* (x+1) mod 4 */

/* Transition tables. */
static unsigned char satisfied_mask[64][64];   /* bits satisfied by next row */
static unsigned char unsat_next[64][64];       /* unsatisfied mask for next row */

/* Build the transition tables once. */
static void init_transition_tables(void)
{
    for (int p = 0; p < 64; ++p) {
        for (int n = 0; n < 64; ++n) {
            int sat = 0;
            int unsat = 0;
            for (int j = 0; j < 3; ++j) {
                /* check if the cell in previous row (p) at column j
                   is satisfied by a +1 neighbour in the next row (n) */
                bool satisfies = false;
                if (val[n][j] == inc[val[p][j]]) satisfies = true;      /* down neighbour */
                if (j > 0 && val[n][j-1] == inc[val[p][j]]) satisfies = true; /* down‑left */
                if (satisfies) sat |= (1 << j);

                /* compute unsatisfied mask for the new row (n) */
                bool sat_cur = false;
                if (j > 0 && val[n][j-1] == inc[val[n][j]]) sat_cur = true; /* left */
                if (j < 2 && val[n][j+1] == inc[val[n][j]]) sat_cur = true; /* right */
                /* up neighbour: its value must equal inc of current */
                if (val[p][j] == inc[val[n][j]]) sat_cur = true; /* up */
                /* up‑right neighbour */
                if (j < 2 && val[p][j+1] == inc[val[n][j]]) sat_cur = true; /* up‑right */
                if (!sat_cur) unsat |= (1 << j);
            }
            satisfied_mask[p][n] = (unsigned char)sat;
            unsat_next[p][n] = (unsigned char)unsat;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static state for memoisation and DP tables */
    static bool tables_ready = false;
    static bool init_done   = false;
    static idx_t max_n_computed = 0;
    static seq_t *cache = NULL;                     /* cache[1..n] */
    static seq_t dp_cur[64][8];
    static seq_t dp_next[64][8];

    /* Initialise everything once */
    if (!tables_ready) {
        /* populate base‑4 digit table */
        for (int c = 0; c < 64; ++c) {
            for (int j = 0; j < 3; ++j) {
                val[c][j] = (c >> (2*j)) & 3;
            }
        }
        init_transition_tables();
        tables_ready = true;
    }

    /* If the requested n is larger than we have computed, extend. */
    if (n > max_n_computed) {
        /* Reallocate cache to hold up to n */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return zero (should not happen) */
            return 0;
        }
        cache = new_cache;

        /* Initialise DP if this is first run */
        if (!init_done) {
            memset(dp_cur, 0, sizeof(dp_cur));
            for (int c = 0; c < 64; ++c) {
                if (val[c][0] != 0) continue;  /* upper-left must be 0 */
                int mask = 0;
                /* unsatisfied mask for the first row (no up neighbours) */
                for (int j = 0; j < 3; ++j) {
                    bool sat = false;
                    if (j > 0 && val[c][j-1] == inc[val[c][j]]) sat = true;
                    if (j < 2 && val[c][j+1] == inc[val[c][j]]) sat = true;
                    if (!sat) mask |= (1 << j);
                }
                dp_cur[c][mask] += (seq_t)1;
            }
            /* compute a(1) */
            seq_t total = 0;
            for (int c = 0; c < 64; ++c) {
                total += dp_cur[c][0];
            }
            cache[1] = total;
            max_n_computed = 1;
            init_done = true;
        }

        /* Extend DP up to row n */
        for (idx_t row = max_n_computed + 1; row <= n; ++row) {
            memset(dp_next, 0, sizeof(dp_next));

            for (int p = 0; p < 64; ++p) {
                for (int u = 0; u < 8; ++u) {
                    seq_t ways = dp_cur[p][u];
                    if (!ways) continue;
                    for (int nxt = 0; nxt < 64; ++nxt) {
                        unsigned char need = satisfied_mask[p][nxt];
                        if ((need & u) != u) continue;   /* some unsatisfied cell not satisfied */
                        unsigned char nxt_unsat = unsat_next[p][nxt];
                        dp_next[nxt][nxt_unsat] += ways;
                    }
                }
            }

            /* swap dp_cur and dp_next */
            for (int c = 0; c < 64; ++c) {
                memcpy(dp_cur[c], dp_next[c], sizeof(dp_cur[c]));
            }

            /* compute a(row) */
            seq_t total = 0;
            for (int c = 0; c < 64; ++c) {
                total += dp_cur[c][0];
            }
            cache[row] = total;
            max_n_computed = row;
        }
    }

    return cache[n];
}
