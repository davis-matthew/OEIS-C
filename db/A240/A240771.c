/*
 * A240771  Number of nX3 0..3 arrays with no element equal to exactly one
 *          horizontal or vertical neighbor, with new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A240771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A240771
 *
 *  Number of n×3 arrays with values 0..3 such that no element is equal to
 *  exactly one of its horizontal or vertical neighbors, with new values 0..3
 *  introduced in row‑major order.
 *
 *  The algorithm keeps a dynamic programming table over the following
 *  state:
 *      (previous row pattern, vertical equality mask, maximum colour used so far)
 *
 *  For each n it extends the DP one row at a time and sums the
 *  states that satisfy the bottom‑row condition to obtain a(n).
 *
 *  The code is ISO C11 compatible and also compiles as C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----- constants ----- */
#define PAT 64          /* 4³ possible row patterns                    */
#define EQ  8           /* 3‑bit mask for vertical matches with above   */
#define MX  4           /* maximum colour index (0..3)                 */
#define STATE (PAT * EQ * MX)

/* ----- tables ----- */
static unsigned char val[PAT][3];
static unsigned char maxv[PAT];
static unsigned char horiz[PAT][3];
static unsigned char eqMask[PAT][PAT];
static unsigned char okTrans[PAT][EQ][PAT];
static unsigned char okBottom[PAT][EQ];
/* patterns_for_max[m] holds all patterns whose first new colour beyond
   m is m+1 (or, for m=4, all patterns that can start a new array).      */
static unsigned char patterns_for_max[5][PAT];
static unsigned char patterns_len[5];

/* ----- table initialisation ----- */
static int tables_initialized = 0;

static void init_tables(void)
{
    if (tables_initialized) return;
    /* decode patterns, compute max colour and horizontal matches */
    for (int p = 0; p < PAT; ++p) {
        int x = p;
        val[p][0] = x & 3; x >>= 2;
        val[p][1] = x & 3; x >>= 2;
        val[p][2] = x & 3;
        maxv[p] = (unsigned char) ( (val[p][0] > val[p][1]) ?
                                   ( (val[p][0] > val[p][2]) ? val[p][0] : val[p][2] )
                                   : ( (val[p][1] > val[p][2]) ? val[p][1] : val[p][2] ) );

        horiz[p][0] = (val[p][0] == val[p][1]) ? 1 : 0;
        horiz[p][2] = (val[p][2] == val[p][1]) ? 1 : 0;
        horiz[p][1] = (val[p][1] == val[p][0]) + (val[p][1] == val[p][2]);
    }

    /* equality mask between two rows (bit j set if equal) */
    for (int p = 0; p < PAT; ++p) {
        for (int q = 0; q < PAT; ++q) {
            unsigned char m = 0;
            for (int j = 0; j < 3; ++j)
                if (val[p][j] == val[q][j]) m |= 1u << j;
            eqMask[p][q] = m;
        }
    }

    /* transition validity: for each (prev pattern, prevEQ, next pattern) */
    for (int p = 0; p < PAT; ++p) {
        for (int a = 0; a < EQ; ++a) {
            for (int q = 0; q < PAT; ++q) {
                unsigned char ok = 1;
                for (int j = 0; j < 3; ++j) {
                    int total = horiz[p][j] + ((a >> j) & 1) + ((val[p][j] == val[q][j]) ? 1 : 0);
                    if (total == 1) { ok = 0; break; }
                }
                okTrans[p][a][q] = ok;
            }
        }
    }

    /* bottom condition (no neighbours below) */
    for (int p = 0; p < PAT; ++p) {
        for (int a = 0; a < EQ; ++a) {
            unsigned char ok = 1;
            for (int j = 0; j < 3; ++j) {
                int total = horiz[p][j] + ((a >> j) & 1);
                if (total == 1) { ok = 0; break; }
            }
            okBottom[p][a] = ok;
        }
    }

    /* patterns that satisfy the new‑colour rule for a given maximum m (0..3) */
    for (int m = 0; m < MX; ++m) {
        patterns_len[m] = 0;
        for (int p = 0; p < PAT; ++p) {
            int cur = m;
            unsigned char good = 1;
            for (int j = 0; j < 3; ++j) {
                int v = val[p][j];
                if (v > cur) {
                    if (v != cur + 1) { good = 0; break; }
                    cur = v;
                }
            }
            if (good) {
                patterns_for_max[m][patterns_len[m]++] = (unsigned char)p;
            }
        }
    }

    /* patterns that can start a new array (no colours seen yet) */
    patterns_len[MX] = 0;                        /* use index 4 for this group */
    for (int p = 0; p < PAT; ++p) {
        int cur = -1;
        unsigned char good = 1;
        for (int j = 0; j < 3; ++j) {
            int v = val[p][j];
            if (v > cur) {
                if (v != cur + 1) { good = 0; break; }
                cur = v;
            }
        }
        if (good) {
            patterns_for_max[MX][patterns_len[MX]++] = (unsigned char)p;
        }
    }

    tables_initialized = 1;
}

/* helper to compute state index */
static inline int state_index(int pat, int a, int m)
{
    return ((pat * EQ + a) * MX + m);
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    /* initialise tables once */
    init_tables();

    /* static working arrays */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    static seq_t *vals = NULL;
    static idx_t vals_cap = 0;
    static idx_t computed_rows = 0;

    /* allocate DP arrays on first call */
    if (!dp_cur) {
        dp_cur  = (seq_t *)calloc(STATE, sizeof(seq_t));
        dp_next = (seq_t *)calloc(STATE, sizeof(seq_t));
        if (!dp_cur || !dp_next) exit(1);
    }

    /* allocate storage for a(n) values */
    if (n >= vals_cap) {
        idx_t new_cap = (n * 2) + 10;                /* grow fast enough */
        seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        vals = tmp;
        vals_cap = new_cap;
    }
    /* initialise dummy entry */
    if (vals[0] != 0) vals[0] = 0;

    /* compute additional rows if necessary */
    while (computed_rows < n) {
        computed_rows++;
        if (computed_rows == 1) {
            /* first row: initialise from start patterns (group index 4) */
            memset(dp_cur, 0, STATE * sizeof(seq_t));
            for (int idx = 0; idx < patterns_len[MX]; ++idx) {
                int p = patterns_for_max[MX][idx];
                int m = maxv[p];
                int state = state_index(p, 0, m);      /* vertical mask 0 */
                dp_cur[state] += (seq_t)1;
            }
        } else {
            /* transition from previous row to current */
            memset(dp_next, 0, STATE * sizeof(seq_t));
            for (int s = 0; s < STATE; ++s) {
                seq_t ways = dp_cur[s];
                if (!ways) continue;
                int tmp = s;
                int m = tmp % MX;  tmp /= MX;
                int a = tmp % EQ;  int p = tmp / EQ;

                /* iterate over admissible next patterns given current max m */
                for (int idx = 0; idx < patterns_len[m]; ++idx) {
                    int q = patterns_for_max[m][idx];
                    if (!okTrans[p][a][q]) continue;
                    int na = eqMask[p][q];
                    int nm = (maxv[q] > m) ? maxv[q] : m;
                    int ns = state_index(q, na, nm);
                    dp_next[ns] += ways;
                }
            }
            memcpy(dp_cur, dp_next, STATE * sizeof(seq_t));
        }

        /* sum states that satisfy the bottom condition */
        seq_t total = (seq_t)0;
        for (int s = 0; s < STATE; ++s) {
            seq_t ways = dp_cur[s];
            if (!ways) continue;
            int tmp = s;
            int m = tmp % MX;  tmp /= MX;
            int a = tmp % EQ;  int p = tmp / EQ;
            if (okBottom[p][a]) total += ways;
        }
        vals[computed_rows] = total;
    }

    return vals[n];
}
