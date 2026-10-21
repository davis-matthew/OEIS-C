/*
 * A208146  Number of 7Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 115, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A208146: Number of 7×n binary arrays avoiding the three‑cell patterns
   0 0 1 and 0 1 1 horizontally and 0 0 1 and 1 0 1 vertically.

   Computation:  Every column is a 7‑bit pattern that avoids the vertical
   forbidden triples.  There are 14 such patterns.  A horizontal triple
   a,b,c is forbidden iff a_bit==0 and c_bit==1, for any row.  Hence a new
   column c is allowed after a iff (c & ~a) == 0 (c is a subset of a),
   regardless of the middle column.  The number of n‑column arrays is
   obtained by dynamic programming over pairs of consecutive columns.
   The DP has state (a,b) = last two column patterns and transitions
   (a,b) → (b,c) for all c that are subsets of a.  Complexity O(n·14²·14)
   which is trivial for n up to 10 000.  The solution is fully exact for
   any seq_t type (fixed‑size or arbitrary precision).
*/

#include <stddef.h>
#include <stdbool.h>

#define HEIGHT 7
#define PAT_COUNT 14   /* number of valid column patterns */

/* Valid column patterns (7‑bit values).  Filled in init_tables(). */
static unsigned char pat[PAT_COUNT];
/* For each pattern a, list of indices c such that (c & ~a)==0 and
   c is a valid pattern.  trans_cnt[i] gives the number of such c. */
static unsigned char trans[PAT_COUNT][PAT_COUNT];
static size_t trans_cnt[PAT_COUNT];

/* Flag to ensure one‑time initialization. */
static bool initialized = false;

static void init_tables(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Enumerate all 7‑bit patterns that avoid the vertical forbidden triples
       001 and 101. */
    size_t idx = 0;
    for (unsigned int p = 0; p < (1U << HEIGHT); ++p) {
        bool ok = true;
        for (int r = 0; r + 2 < HEIGHT; ++r) {
            int a = (p >> r) & 1;
            int b = (p >> (r + 1)) & 1;
            int c = (p >> (r + 2)) & 1;
            if ((a == 0 && b == 0 && c == 1) ||
                (a == 1 && b == 0 && c == 1)) {
                ok = false;
                break;
            }
        }
        if (ok && idx < PAT_COUNT)
            pat[idx++] = (unsigned char)p;
    }

    /* Build transition lists: for each pattern a, all patterns c with
       c subset of a.  Subset condition equivalently: (c & ~a)==0. */
    for (size_t i = 0; i < PAT_COUNT; ++i) {
        unsigned char a = pat[i];
        size_t cnt = 0;
        for (size_t j = 0; j < PAT_COUNT; ++j) {
            unsigned char c = pat[j];
            if ((c & ~a) == 0) {
                trans[i][cnt++] = (unsigned char)j;
            }
        }
        trans_cnt[i] = cnt;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    init_tables();

    /* Base cases */
    if (n == 1)  /* one column: any valid pattern */
        return (seq_t)PAT_COUNT;         /* 14 */
    if (n == 2)  /* two columns: any pair of valid patterns */
        return (seq_t)PAT_COUNT * (seq_t)PAT_COUNT;  /* 14*14 = 196 */

    /* DP over pairs of consecutive columns.  dp[a][b] = number of ways
       to fill up to current column with last two columns pattern indices a,b. */
    static seq_t dp[PAT_COUNT][PAT_COUNT];
    static seq_t newdp[PAT_COUNT][PAT_COUNT];
    /* Initialise for first two columns: all pairs are allowed. */
    for (size_t a = 0; a < PAT_COUNT; ++a)
        for (size_t b = 0; b < PAT_COUNT; ++b)
            dp[a][b] = 1;

    /* Iterate through columns 3..n */
    for (idx_t col = 3; col <= n; ++col) {
        /* Zero newdp */
        for (size_t i = 0; i < PAT_COUNT; ++i)
            for (size_t j = 0; j < PAT_COUNT; ++j)
                newdp[i][j] = 0;

        /* Transition: (a,b) → (b,c) for every c that is subset of a */
        for (size_t a = 0; a < PAT_COUNT; ++a) {
            for (size_t b = 0; b < PAT_COUNT; ++b) {
                seq_t val = dp[a][b];
                if (val == 0)
                    continue;
                size_t cnt = trans_cnt[a];
                for (size_t k = 0; k < cnt; ++k) {
                    size_t c = trans[a][k];
                    newdp[b][c] += val;
                }
            }
        }
        /* Swap dp and newdp */
        for (size_t i = 0; i < PAT_COUNT; ++i)
            for (size_t j = 0; j < PAT_COUNT; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum all states to obtain total count for n columns */
    seq_t result = 0;
    for (size_t a = 0; a < PAT_COUNT; ++a)
        for (size_t b = 0; b < PAT_COUNT; ++b)
            result += dp[a][b];
    return result;
}
