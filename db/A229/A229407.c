/*
 * A229407  Number of nX3 0..3 arrays avoiding 11 horizontally, 22 vertically,
 *          33 antidiagonally and 00 diagonally.
 * https://oeis.org/A229407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of n×3 0..3 arrays avoiding
   11 horizontally, 22 vertically, 33 antidiagonally and 00 diagonally.
   This implementation counts arrays by dynamic programming over all
   64 possible row patterns (each row is a 3‑digit base‑4 number). */ 
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system.  No typedefs here. */

enum { PAT_COUNT = 64 };           /* 4^3 possible rows */

/* Static tables for the pattern restrictions.  They are built at first
   call of the function. */
static bool initialized = false;
static unsigned char digits[PAT_COUNT][3];
static bool horiz_ok[PAT_COUNT];
static bool trans_ok[PAT_COUNT][PAT_COUNT];

seq_t a(idx_t n)
{
    /* Build transition tables on first call.  The arrays are of fixed
       size so the compiler can allocate them in the data segment. */
    if (!initialized) {
        for (int p = 0; p < PAT_COUNT; ++p) {
            digits[p][0] = (unsigned char)((p        ) & 3);
            digits[p][1] = (unsigned char)((p >> 2) & 3);
            digits[p][2] = (unsigned char)((p >> 4) & 3);

            /* horizontal restriction: disallow adjacent 11 */
            horiz_ok[p] = !((digits[p][0] == 1 && digits[p][1] == 1) ||
                            (digits[p][1] == 1 && digits[p][2] == 1));
        }

        /* Transition between successive rows.  A transition is allowed
           iff the vertical, antidiagonal, diagonal and horizontal
           restrictions are satisfied. */
        for (int p = 0; p < PAT_COUNT; ++p) {
            if (!horiz_ok[p]) {
                for (int q = 0; q < PAT_COUNT; ++q)
                    trans_ok[p][q] = false;
                continue;
            }
            for (int q = 0; q < PAT_COUNT; ++q) {
                if (!horiz_ok[q]) {
                    trans_ok[p][q] = false;
                    continue;
                }
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    /* vertical 22 */
                    if (digits[p][j] == 2 && digits[q][j] == 2) { ok = false; break; }
                    /* antidiagonal 33 */
                    if (j > 0 && digits[p][j] == 3 && digits[q][j-1] == 3) { ok = false; break; }
                    /* diagonal 00 */
                    if (j < 2 && digits[p][j] == 0 && digits[q][j+1] == 0) { ok = false; break; }
                }
                trans_ok[p][q] = ok;
            }
        }
        initialized = true;
    }

    /* Dynamic programming over rows.  dp[i] counts the number of valid
       arrays of the current height that end with pattern i. */
    seq_t dp[PAT_COUNT];
    seq_t dp_next[PAT_COUNT];

    /* Height 1: each pattern that satisfies the horizontal rule is a
       valid single‑row array. */
    for (int i = 0; i < PAT_COUNT; ++i)
        dp[i] = horiz_ok[i] ? (seq_t)1 : (seq_t)0;

    /* Extend the array row by row up to height n. */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < PAT_COUNT; ++i)
            dp_next[i] = (seq_t)0;
        for (int p = 0; p < PAT_COUNT; ++p) {
            seq_t val = dp[p];
            if (!val) continue;
            for (int q = 0; q < PAT_COUNT; ++q) {
                if (trans_ok[p][q])
                    dp_next[q] += val;
            }
        }
        for (int i = 0; i < PAT_COUNT; ++i)
            dp[i] = dp_next[i];
    }

    /* Sum over all ending patterns to obtain the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < PAT_COUNT; ++i)
        total += dp[i];

    return total;
}
