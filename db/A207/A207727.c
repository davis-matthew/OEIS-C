/*
 * A207727  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A207727: Number of n×6 0..1 arrays avoiding
 *   horizontally 000 and 010,
 *   vertically 011 and 101.
 *
 * This code uses a finite‑state automaton with
 *   states = ordered pairs of 6‑bit rows.
 * States are indexed by (row1,row2).  A transition
 *   from (a,b) to (b,c) is allowed iff for each
 *   column the 3‑bit vertical pattern (row[a],row[b],row[c])
 *   is not 011 or 101.  Each row must itself avoid
 *   horizontal patterns 000 and 010.
 * The number of valid rows is 25; hence there are 625 states.
 *
 * count(n):  m                               for n=1  (25)
 *            m*m                            for n=2  (625)
 *            Σ_{states} DP_{n}[state]       for n≥3
 *
 * The DP uses only additions; all intermediate data
 * fit into the arbitrary‑precision `seq_t` type.
 *
 * Complexity : O(n·m²) = O(n) with m=25 (≈ 1.5 × 10⁴ ops per row).
 * Memory    : few kilobytes.
 */

#include <stdint.h>

static const int ROW_BSLEN = 6;      /* number of columns */
static const int MAX_ROWS  = 1 << ROW_BSLEN;   /* 64 */

static int          m;              /* number of valid rows (25) */
static uint8_t      validRows[64];  /* masks of valid rows (0..63) */
static int          maskToIdx[64];  /* mapping mask→index, -1 if invalid */
static uint8_t      transTo[625][25];   /* transitions from each state */
static uint8_t      transCnt[625];      /* number of next rows */

/* check whether a 6‑bit row avoids horizontal patterns 000 and 010 */
static int rowValid(uint8_t mask)
{
    for (int shift = 0; shift <= ROW_BSLEN - 3; ++shift) {
        int sub = (mask >> shift) & 7;          /* 3‑bit window */
        if (sub == 0 || sub == 2)              /* 000 or 010 forbidden */
            return 0;
    }
    return 1;
}

/* verify if adding a third row creates a forbidden vertical triple */
static int verticalTripleOK(uint8_t r1, uint8_t r2, uint8_t r3)
{
    for (int col = 0; col < ROW_BSLEN; ++col) {
        int pattern =
            (((r1 >> col) & 1) << 2) |
            (((r2 >> col) & 1) << 1) |
            (((r3 >> col) & 1));
        if (pattern == 3 || pattern == 5)       /* 011 or 101 forbidden */
            return 0;
    }
    return 1;
}

/* initialise row set and transition table only once */
static void initTables(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* build list of valid rows and masks→index map */
    for (int i = 0; i < MAX_ROWS; ++i) {
        maskToIdx[i] = -1;
    }
    m = 0;
    for (int mask = 0; mask < MAX_ROWS; ++mask) {
        if (rowValid((uint8_t)mask)) {
            validRows[m] = (uint8_t)mask;
            maskToIdx[mask] = m;
            ++m;              /* expect m == 25 */
        }
    }

    /* build transition lists for all state pairs (a,b) */
    for (int a = 0; a < m; ++a) {
        for (int b = 0; b < m; ++b) {
            int state = a * m + b;
            transCnt[state] = 0;
            uint8_t r1 = validRows[a];
            uint8_t r2 = validRows[b];
            for (int c = 0; c < m; ++c) {
                uint8_t r3 = validRows[c];
                if (verticalTripleOK(r1, r2, r3)) {
                    transTo[state][transCnt[state]++] = (uint8_t)c;
                }
            }
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    initTables();

    if (n == 0) {                 /* undefined in OEIS, return 0 */
        return (seq_t)0;
    }

    if (n == 1) {                 /* single row */
        return (seq_t)m;
    }

    if (n == 2) {                 /* two rows, no vertical restriction yet */
        return (seq_t)m * (seq_t)m;
    }

    /* DP arrays: current counts and next counts for each state */
    static seq_t dp[625];
    static seq_t next[625];
    for (int i = 0; i < m * m; ++i) dp[i] = (seq_t)0;

    /* initialise for 2 rows: every pair is allowed once */
    for (int a = 0; a < m; ++a) {
        for (int b = 0; b < m; ++b) {
            dp[a * m + b] = (seq_t)1;
        }
    }

    /* iterate rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < m * m; ++i) next[i] = (seq_t)0;

        for (int state = 0; state < m * m; ++state) {
            seq_t cur = dp[state];
            if (cur == 0) continue;
            int lastRow = state % m;          /* b in (a,b) */
            /* for each possible new row c */
            for (uint8_t t = 0; t < transCnt[state]; ++t) {
                uint8_t newRow = transTo[state][t];
                int newState = lastRow * m + newRow; /* (b,c) */
                next[newState] += cur;
            }
        }

        /* move to next iteration */
        for (int i = 0; i < m * m; ++i) dp[i] = next[i];
    }

    /* sum all sequences of length n */
    seq_t total = (seq_t)0;
    for (int i = 0; i < m * m; ++i) total += dp[i];
    return total;
}
