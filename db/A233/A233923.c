/*
 * A233923  Number of (n+1)X(3+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 8
 *          and no adjacent elements equal.
 * https://oeis.org/A233923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A233923
 *
 *  The number of (n+1)×4 0..4 arrays with no equal adjacent entries
 *  (horizontally or vertically) such that every 2×2 subblock has
 *  the sum of absolute differences of the four edge pairs equal to 8.
 *
 *  Implementation: dynamic programming over the 320 admissible states
 *  of a single row.  An adjacency matrix records which states may
 *  follow each other; for each n we run one transition step.
 *
 *  Complexity: O(n·S²) in the naive transition, here
 *  O(n·S·avg_out_deg) = O(n·S²) with S = 320, which easily satisfies
 *  the time limits for n up to ~10 000.
 */

#include <stdbool.h>
#include <stdint.h>

/* ------------------------------ globals ------------------------------ */
#define MAX_STATES 320            /* 5 * 4³ */
#define MAX_N     20000           /* safe upper bound for caching */
static unsigned char state_digit[MAX_STATES][4];
static int state_code[MAX_STATES];
static int state_map[625];          /* 5⁴ */

static unsigned char out_deg[MAX_STATES];
static int out_edge[MAX_STATES][MAX_STATES];

static seq_t dp_cur[MAX_STATES];
static seq_t dp_next[MAX_STATES];

static seq_t a_vals[MAX_N + 1];
static idx_t max_computed = 0;
static bool tables_built = false;
static int S = 0;                  /* actual number of states */

/* --------------------------- build table --------------------------- */
static void build_tables(void)
{
    int i, j, k, l, code, idx = 0;

    /* initialise state map */
    for (i = 0; i < 625; ++i)
        state_map[i] = -1;

    /* enumerate all states with no equal horizontal neighbours */
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 5; ++j) if (j != i)
            for (k = 0; k < 5; ++k) if (k != j)
                for (l = 0; l < 5; ++l) if (l != k) {
                    code = (((i * 5 + j) * 5 + k) * 5 + l);
                    state_map[code] = idx;
                    state_digit[idx][0] = (unsigned char)i;
                    state_digit[idx][1] = (unsigned char)j;
                    state_digit[idx][2] = (unsigned char)k;
                    state_digit[idx][3] = (unsigned char)l;
                    state_code[idx] = code;
                    ++idx;
                }
    S = idx;

    /* build adjacency lists */
    for (i = 0; i < S; ++i) {
        unsigned char *si = state_digit[i];
        out_deg[i] = 0;
        for (j = 0; j < S; ++j) {
            unsigned char *tj = state_digit[j];
            bool ok = true;
            /* vertical adjacency: no equal neighbours */
            for (k = 0; k < 4 && ok; ++k)
                if (si[k] == tj[k]) ok = false;
            if (!ok) continue;
            /* 2×2 block constraints */
            for (k = 0; k < 3 && ok; ++k) {
                int s1 = si[k], s2 = si[k+1];
                int t1 = tj[k], t2 = tj[k+1];
                int sum = (s1 > s2 ? s1 - s2 : s2 - s1) +
                          (t1 > t2 ? t1 - t2 : t2 - t1) +
                          (s1 > t1 ? s1 - t1 : t1 - s1) +
                          (s2 > t2 ? s2 - t2 : t2 - s2);
                if (sum != 8) ok = false;
            }
            if (ok) {
                int d = out_deg[i];
                out_edge[i][d] = j;
                ++out_deg[i];
            }
        }
    }

    /* initialise DP: one way for each possible first row */
    for (i = 0; i < S; ++i)
        dp_cur[i] = 1;

    /* initialise cache */
    a_vals[0] = 0;          /* unused */
    max_computed = 0;

    tables_built = true;
}

/* ----------------------------- main -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!tables_built)
        build_tables();

    /* compute up to requested n if necessary */
    if (n > max_computed) {
        idx_t step;
        for (step = max_computed + 1; step <= n; ++step) {
            int i, s;
            /* reset next dp */
            for (i = 0; i < S; ++i)
                dp_next[i] = 0;

            /* transition */
            for (s = 0; s < S; ++s) {
                seq_t val = dp_cur[s];
                if (val == 0) continue;
                unsigned char d = out_deg[s];
                for (i = 0; i < d; ++i) {
                    int t = out_edge[s][i];
                    dp_next[t] += val;
                }
            }

            /* total arrays of height step+1 (since n counts rows-1) */
            seq_t total = 0;
            for (i = 0; i < S; ++i)
                total += dp_next[i];

            a_vals[step] = total;
            /* prepare for next iteration */
            for (i = 0; i < S; ++i)
                dp_cur[i] = dp_next[i];
        }
        max_computed = n;
    }

    return a_vals[n];
}
