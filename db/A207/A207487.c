/*
 * A207487  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays avoiding horizontal patterns
 *            000 and 011 and vertical patterns 011 and 110. Computation:
 *            dynamic programming on the last two rows with a transfer‑matrix
 *            built from the 25 admissible 7‑bit rows
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define H 25                /* number of admissible 7‑bit rows */
#define STATES (H*H)        /* number of states (last two rows) */

/* global tables, initialized once */
static int  row_bits[H];                          /* admissible row patterns */
static int  trans[STATES][H];                     /* transitions matrix */
static int  trans_cnt[STATES];                    /* number of transitions per state */

/* check a 7‑bit number for the horizontal constraints */
static int valid_row(int bits)
{
    int j;
    for (j = 0; j <= 4; ++j) {
        int shift   = 4 - j;          /* triple starts at bit 6-j..4-j */
        int triple  = (bits >> shift) & 7;
        if (triple == 0 || triple == 3)   /* 000 or 011 forbidden */
            return 0;
    }
    return 1;
}

/* one‑time init of tables */
static void init_tables(void)
{
    /* build list of admissible rows */
    int idx = 0;
    int bits;
    for (bits = 0; bits < 128; ++bits) {
        if (valid_row(bits))
            row_bits[idx++] = bits;
    }

    /* pre‑compute transitions */
    int ai, bi, ci;
    for (ai = 0; ai < H; ++ai) {
        int a_bits = row_bits[ai];
        for (bi = 0; bi < H; ++bi) {
            int b_bits   = row_bits[bi];
            int mask_b   = b_bits;
            int eq_bits  = a_bits & mask_b;
            int s        = ai * H + bi;
            int tcnt     = 0;
            for (ci = 0; ci < H; ++ci) {
                int c_bits = row_bits[ci];
                if ((c_bits & mask_b) == eq_bits)
                    trans[s][tcnt++] = bi * H + ci;
            }
            trans_cnt[s] = tcnt;
        }
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    /* initialise tables on first call */
    static int initialized = 0;
    if (!initialized) {
        init_tables();
        initialized = 1;
    }

    if (n == 1) return (seq_t)H;
    if (n == 2) return (seq_t)H * (seq_t)H;

    static seq_t cur[STATES];
    static seq_t nxt[STATES];
    int s;

    /* initial state: all pairs are possible for the first two rows */
    for (s = 0; s < STATES; ++s) cur[s] = 1;

    /* DP from row 3 up to n */
    idx_t step;
    for (step = 3; step <= n; ++step) {
        /* zero next vector */
        for (s = 0; s < STATES; ++s) nxt[s] = 0;

        for (s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            int tcnt = trans_cnt[s];
            int *tarr = trans[s];
            int k;
            for (k = 0; k < tcnt; ++k)
                nxt[tarr[k]] += val;
        }

        /* copy back */
        for (s = 0; s < STATES; ++s) cur[s] = nxt[s];
    }

    /* sum all final states */
    seq_t total = 0;
    for (s = 0; s < STATES; ++s) total += cur[s];
    return total;
}
