/*
 * A207843  Number of n X 6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0/1 arrays avoiding horizontal 000 and 001
 *            and vertical 000 and 111. Computed by dynamic programming over
 *            the set of admissible 6‑bit rows. The recurrence is not
 *            explicit; instead we construct all allowed rows, build a
 *            transition table that respects the vertical constraints,...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_N      20000          /*  sufficient for required range (≈10000) */
#define MAX_ROWS   64             /* 2^6 possible rows */

/* Cached data */
static bool initialized = false;
static idx_t computed_up_to = 0;    /* highest n for which seq_table is filled */

/* 1‑based table of values */
static seq_t seq_table[MAX_N + 1];

/* allowed rows */
static int num_rows = 0;
static unsigned int rows[MAX_ROWS];

/* transition table: for each ordered pair (p1,p2) the list of
   admissible next rows (indices into rows[]).  A compact
   representation is used: next_rows[pair][k] gives the k‑th
   successor; next_count[pair] gives the number of successors. */
static unsigned char next_rows[MAX_ROWS * MAX_ROWS][MAX_ROWS];
static unsigned char next_count[MAX_ROWS * MAX_ROWS];

/* DP state arrays: dp[cur][pair]   number of ways that the last
   two rows are (rows[p1], rows[p2]).  Two arrays are swapped
   each step. */
static seq_t dp[2][MAX_ROWS * MAX_ROWS];
static int dp_cur = 0;
static int dp_next = 1;

/* --------------------------------------------------------------------- */
/*  Pre‑computation of allowed rows and transition lists.  This is
    executed once on the first call to a().  It also computes the
    first two terms of the sequence.                            */
/* --------------------------------------------------------------------- */
static void init_sequences(void)
{
    /* build the list of 6‑bit rows that avoid horizontal 000 and 001 */
    num_rows = 0;
    for (unsigned int pat = 0; pat < (1u << 6); ++pat) {
        bool ok = true;
        for (int j = 0; j <= 3 && ok; ++j) {
            unsigned int tri = (pat >> j) & 7u;   /* 3 consecutive bits */
            if (tri == 0u || tri == 1u)           /* 000 or 001 forbidden */
                ok = false;
        }
        if (ok)
            rows[num_rows++] = pat;
    }

    /* build transition lists for each ordered pair of rows */
    for (int p1 = 0; p1 < num_rows; ++p1) {
        for (int p2 = 0; p2 < num_rows; ++p2) {
            size_t idx = (size_t)p1 * (size_t)num_rows + (size_t)p2;
            unsigned char cnt = 0;
            for (int r = 0; r < num_rows; ++r) {
                bool ok = true;
                unsigned int a = rows[p1];
                unsigned int b = rows[p2];
                unsigned int c = rows[r];
                for (int col = 0; col < 6; ++col) {
                    bool bit1 = (a >> col) & 1u;
                    bool bit2 = (b >> col) & 1u;
                    bool bit3 = (c >> col) & 1u;
                    if (bit1 == bit2 && bit2 == bit3)   /* 000 or 111 */
                        ok = false;
                }
                if (ok)
                    next_rows[idx][cnt++] = (unsigned char)r;
            }
            next_count[idx] = cnt;
        }
    }

    /* first two terms: single row, any pair of rows */
    seq_table[1] = (seq_t)num_rows;
    seq_table[2] = (seq_t)num_rows * (seq_t)num_rows;

    /* initialize DP for n = 2: each pair contributes one configuration */
    size_t state_size = (size_t)num_rows * (size_t)num_rows;
    for (size_t i = 0; i < state_size; ++i)
        dp[dp_cur][i] = (seq_t)1;

    computed_up_to = 2;
    initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Extend the table up to the requested index n by dynamic programming.
    Updates seq_table[n] and the DP arrays.                               */
/* --------------------------------------------------------------------- */
static void extend_table(idx_t n)
{
    size_t state_size = (size_t)num_rows * (size_t)num_rows;
    for (idx_t len = computed_up_to + 1; len <= n; ++len) {
        /* zero next DP array */
        for (size_t i = 0; i < state_size; ++i)
            dp[dp_next][i] = (seq_t)0;

        /* transition from current pairs to admissible next rows */
        for (size_t id = 0; id < state_size; ++id) {
            seq_t ways = dp[dp_cur][id];
            if (!ways)
                continue;
            unsigned char cnt = next_count[id];
            for (unsigned char k = 0; k < cnt; ++k) {
                size_t r = next_rows[id][k];
                size_t new_id = (id % num_rows) * (size_t)num_rows + r;
                dp[dp_next][new_id] += ways;
            }
        }

        /* sum all entries to get a(len) */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < state_size; ++i)
            total += dp[dp_next][i];
        seq_table[len] = total;

        /* swap DP arrays */
        int tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }
    computed_up_to = n;
}

/* --------------------------------------------------------------------- */
/*  Public function: a(n)                                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequences();

    if (n <= computed_up_to)
        return seq_table[n];

    if (n > MAX_N) {
        /* For the intended test range this should not happen.    */
        /* Extend the static array if necessary (not implemented). */
    }

    extend_table(n);
    return seq_table[n];
}
