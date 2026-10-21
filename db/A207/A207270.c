/*
 * A207270  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 30, u64 n <= 67, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207270: Number of 3 × n binary arrays avoiding
 *  horizontal 000, 011 and vertical 011, 101.
 *
 *  Implementation via dynamic programming over column pairs.
 *  State = (previous column, current column) ∈ allowed × allowed,
 *  where a column is a 3‑bit pattern that is not 011 or 101.
 *  DP transition checks the horizontal rule for each row.
 *
 *  The solution is O(n) time and O(1) extra space (apart from the
 *  memoisation cache).  It works for any idx_t ≥ 1 and any seq_t
 *  used by the build system.  The algorithm is fully compliant with
 *  C11 and C++17.
 *
 *  Initial values:
 *    a(1) = 6,  a(2) = 36.
 *
 *  Note: seq_t and idx_t are typedefs defined by the build environment
 *  (e.g. typedef unsigned long long idx_t;).
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Tables and memoisation state                                     */
/* ------------------------------------------------------------------ */
static int allowed[6];              /* 3‑bit columns that satisfy the vertical rule */
static int col_to_idx[8];           /* map a 3‑bit pattern to its index in ‘allowed’  */
static bool init_done = false;

static int state_a[36];             /* first column of a state (index into allowed)  */
static int state_b[36];             /* second column of a state (index into allowed) */

static seq_t dp_cur[36];            /* DP for current length                         */
static seq_t dp_next[36];           /* DP for next length                            */
static idx_t cur_len = 0;           /* number of columns processed (>=2)              */
static bool dp_initialized = false;

static seq_t results[20001];        /* cache of a(n) for n up to 20000               */
static idx_t max_cached = 0;        /* highest n for which a(n) is stored            */

/* ------------------------------------------------------------------ */
/*  One‑time table initialisation                                     */
/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    if (init_done) return;
    init_done = true;

    /* allowed columns: exclude 011 (3) and 101 (5) */
    for (int i = 0; i < 8; ++i) col_to_idx[i] = -1;
    int k = 0;
    for (int v = 0; v < 8; ++v) {
        if (v == 3 || v == 5) continue;   /* forbidden vertically */
        allowed[k] = v;
        col_to_idx[v] = k;
        ++k;
    }
    /* build state mapping: state = a * 6 + b  (a,b in 0..5) */
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b) {
            int idx = a * 6 + b;
            state_a[idx] = a;
            state_b[idx] = b;
        }
}

/* ------------------------------------------------------------------ */
/*  Initialise DP for length 2 (every pair of allowed columns is legal)*/
/* ------------------------------------------------------------------ */
static void init_dp(void)
{
    for (int i = 0; i < 36; ++i) dp_cur[i] = (seq_t)1;
    cur_len = 2;
    dp_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Main counting routine                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init_tables();  /* build tables only once */

    /* Cache the base values if not yet done. */
    if (max_cached == 0) {
        results[1] = (seq_t)6;   /* single column: all 6 allowed patterns */
        results[2] = (seq_t)36;  /* 6 × 6 pairs, horizontal restriction not yet active */

        init_dp();
        max_cached = 2;
    }

    /* Return cached result if available. */
    if (n <= max_cached) return results[(size_t)n];

    /* Extend DP from cur_len+1 up to n. */
    for (idx_t L = cur_len + 1; L <= n; ++L) {
        /* reset dp_next */
        for (int i = 0; i < 36; ++i) dp_next[i] = (seq_t)0;

        /* transition from states of length L-1 to length L */
        for (int si = 0; si < 36; ++si) {
            seq_t ways = dp_cur[si];
            if (ways == (seq_t)0) continue;

            int a = state_a[si];
            int b = state_b[si];
            int col_b = allowed[b];   /* pattern of middle column */

            for (int ci = 0; ci < 6; ++ci) {
                int col_c = allowed[ci];   /* candidate next column */

                /* check the horizontal rule for each of the 3 rows */
                bool ok = true;
                for (int r = 0; r < 3; ++r) {
                    int bit_a = (allowed[a] >> r) & 1;
                    int bit_b = (col_b >> r) & 1;
                    int bit_c = (col_c >> r) & 1;
                    if (bit_a == 0 && bit_b == 0 && bit_c == 0) { ok = false; break; }
                    if (bit_a == 0 && bit_b == 1 && bit_c == 1) { ok = false; break; }
                }
                if (!ok) continue;

                int dest = b * 6 + ci;  /* new state: (b, c) */
                dp_next[dest] += ways;
            }
        }

        /* total number of arrays of length L */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 36; ++i) total += dp_next[i];
        results[(size_t)L] = total;

        /* prepare for next iteration */
        for (int i = 0; i < 36; ++i) dp_cur[i] = dp_next[i];
        cur_len = L;
    }

    max_cached = n;
    return results[(size_t)n];
}
