/*
 * A185831  Half the number of nX5 binary arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A185831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185831: counts half the number of n×5 binary arrays in which
 * every element equals exactly one or two of its horizontal and vertical
 * neighbors.  The value is computed by dynamic programming over
 * successive rows.  The result for a given n is independent of previously
 * computed values and requires only O(32³) operations per row. */
 
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Test whether a single row satisfies the local constraint,
 * given the presence or absence of vertical neighbours. */
static bool row_ok(uint8_t row,
                   bool up_exists, uint8_t uprow,
                   bool down_exists, uint8_t downrow)
{
    for (int j = 0; j < 5; ++j) {
        int bit = (row >> j) & 1;
        int cnt = 0;

        if (up_exists   && ((uprow   >> j) & 1) == bit) cnt++;
        if (down_exists && ((downrow >> j) & 1) == bit) cnt++;
        if (j > 0   && ((row >> (j-1)) & 1) == bit) cnt++;
        if (j < 4   && ((row >> (j+1)) & 1) == bit) cnt++;

        if (cnt < 1 || cnt > 2) return false;
    }
    return true;
}

/* Tables containing pre–computed validity information. */
static bool val_single[32];
static bool val_start[32][32];
static bool val_last[32][32];
static bool internal_ok[32][32][32];
static bool tables_inited = false;

/* Initialise the tables once. */
static void init_tables(void)
{
    if (tables_inited) return;

    for (int p = 0; p < 32; ++p)
        val_single[p] = row_ok((uint8_t)p, false, 0, false, 0);

    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b) {
            val_start[a][b] = row_ok((uint8_t)a, false, 0, true, (uint8_t)b);
            val_last [a][b] = row_ok((uint8_t)b, true,  (uint8_t)a, false, 0);
        }

    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            for (int x = 0; x < 32; ++x)
                internal_ok[a][b][x] =
                    row_ok((uint8_t)b, true, (uint8_t)a, true, (uint8_t)x);

    tables_inited = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined only for n ≥ 1 */

    init_tables();

    /* Special case: n = 1 */
    if (n == 1) {
        seq_t total = 0;
        for (int p = 0; p < 32; ++p)
            if (val_single[p]) total += (seq_t)1;
        return total / (seq_t)2;
    }

    /* DP arrays holding counts for the last two rows. */
    static seq_t dp_cur[32*32];
    static seq_t dp_next[32*32];

    /* Initialise DP for the first two rows. */
    for (int i = 0; i < 32*32; ++i) dp_cur[i] = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            if (val_start[a][b]) dp_cur[a*32 + b] = (seq_t)1;

    /* If n == 2 we can finish here. */
    if (n == 2) {
        seq_t total = 0;
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                if (val_last[a][b]) total += dp_cur[a*32 + b];
        return total / (seq_t)2;
    }

    /* Extend the DP up to n rows. */
    for (idx_t rows = 3; rows <= n; ++rows) {
        for (int i = 0; i < 32*32; ++i) dp_next[i] = 0;

        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b) {
                seq_t val = dp_cur[a*32 + b];
                if (!val) continue;
                for (int x = 0; x < 32; ++x)
                    if (internal_ok[a][b][x])
                        dp_next[b*32 + x] += val;
            }

        for (int i = 0; i < 32*32; ++i) dp_cur[i] = dp_next[i];
    }

    /* Compute the final result. */
    seq_t total = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            if (val_last[a][b]) total += dp_cur[a*32 + b];

    return total / (seq_t)2;
}
