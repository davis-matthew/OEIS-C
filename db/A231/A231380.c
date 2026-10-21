/*
 * A231380  Number of nX6 0..1 arrays with no element less than a strict
 *          majority of its horizontal and vertical neighbors.
 * https://oeis.org/A231380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation counts n×6 0/1 arrays in which no element is less
   than a strict majority of its horizontal and vertical neighbors.
   The allowed configurations for a row depend on its upper and lower
   neighbor rows; a dynamic programming of two‑row states is used. */

#include <stdbool.h>
#include <stddef.h>

static const int NONE = 64;           /* sentinel for missing neighbor */

/* allowed[above][mid][below] == true iff the middle row 'mid'
   satisfies the local rule when the upper row is 'above'
   (use NONE for missing) and the lower row is 'below' (NONE for missing). */
static bool allowed[65][64][65];

/* For every pair of rows (a,b) the set of rows c that can follow
   while making b satisfy its constraints with a as the upper row. */
static uint64_t nextMask[64][64];

static bool tables_initialized = false;

static void init_tables(void)
{
    if (tables_initialized) return;
    tables_initialized = true;

    for (int above = 0; above <= NONE; ++above) {
        for (int mid = 0; mid < 64; ++mid) {
            for (int below = 0; below <= NONE; ++below) {
                bool ok = true;
                for (int j = 0; j < 6 && ok; ++j) {
                    int val = (mid >> j) & 1;
                    int neigh = 0;
                    int higher = 0;

                    /* left neighbor */
                    if (j > 0) {
                        ++neigh;
                        int nb = (mid >> (j-1)) & 1;
                        if (val == 0 && nb == 1) ++higher;
                    }
                    /* right neighbor */
                    if (j < 5) {
                        ++neigh;
                        int nb = (mid >> (j+1)) & 1;
                        if (val == 0 && nb == 1) ++higher;
                    }
                    /* upper neighbor */
                    if (above != NONE) {
                        ++neigh;
                        int nb = (above >> j) & 1;
                        if (val == 0 && nb == 1) ++higher;
                    }
                    /* lower neighbor */
                    if (below != NONE) {
                        ++neigh;
                        int nb = (below >> j) & 1;
                        if (val == 0 && nb == 1) ++higher;
                    }
                    if (higher > neigh/2) ok = false;
                }
                allowed[above][mid][below] = ok;
            }
        }
    }

    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            uint64_t bits = 0;
            for (int c = 0; c < 64; ++c) {
                if (allowed[a][b][c]) bits |= 1ULL << c;
            }
            nextMask[a][b] = bits;
        }
    }
}

/* seq_t and idx_t are supplied by the build environment.  */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return (seq_t)0;

    /* Special handling for n == 1: only one row, no vertical neighbors. */
    if (n == 1) {
        seq_t total = (seq_t)0;
        for (int r = 0; r < 64; ++r)
            if (allowed[NONE][r][NONE]) ++total;
        return total;
    }

    static seq_t dp[64][64];
    static seq_t dp_next[64][64];

    /* initialise dp with the first two rows */
    for (int a = 0; a < 64; ++a)
        for (int b = 0; b < 64; ++b)
            dp[a][b] = (allowed[NONE][a][b]) ? (seq_t)1 : (seq_t)0;

    /* add rows 3..n */
    for (idx_t step = 2; step < n; ++step) {
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                dp_next[a][b] = 0;

        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                seq_t val = dp[a][b];
                if (!val) continue;
                uint64_t bits = nextMask[a][b];
                while (bits) {
                    uint64_t lsb = bits & -bits;
                    int c = __builtin_ctzll(bits);
                    dp_next[b][c] += val;
                    bits ^= lsb;
                }
            }
        }

        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* finalize by checking the last row (below is missing) */
    seq_t result = (seq_t)0;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            seq_t val = dp[a][b];
            if (!val) continue;
            if (allowed[a][b][NONE]) result += val;
        }
    }
    return result;
}
