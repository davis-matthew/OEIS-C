/*
 * A183844  Number of (n+1) X 9 binary arrays with each element of every 2 X 2
 *          subblock being the sum mod 2 of two others.
 * https://oeis.org/A183844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) × 9 binary arrays with each element of
 *            every * 2×2 subblock being the sum mod 2 of two others. The
 *            array is counted * for n ≥ 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a dynamic-programming approach on the 9‑bit rows.  */
#include <stdint.h>
#include <stdbool.h>

/* Allowed 2×2 patterns over GF(2).
 * Index = (a<<3)|(b<<2)|(c<<1)|d, where a,b are the top row and c,d the
 * bottom row of the block.  The pattern is allowed iff every element of
 * the 2×2 subblock equals the sum mod 2 of two other elements. */
static const unsigned char PATTERN_ALLOWED[16] = {
    1,0,0,1,0,1,1,1,0,1,1,1,1,1,1,0
};

/* Adjacency map: NEXTS[s][k] gives a row `t` that can legally follow a
 * previous row `s`.  COUNT[s] stores how many such successors exist.  */
static unsigned short NEXTS[512][512];
static unsigned char COUNT[512];
static bool INITIALIZED = false;

/* Build the adjacency lists the first time the function is called. */
static void init_adjacency(void)
{
    if (INITIALIZED) return;
    int s, t, j;
    for (s = 0; s < 512; ++s) {
        unsigned char c = 0;
        for (t = 0; t < 512; ++t) {
            int ok = 1;
            for (j = 0; j < 8; ++j) {
                int a = (s >> (8 - j)) & 1;
                int b = (s >> (7 - j)) & 1;
                int cbit = (t >> (8 - j)) & 1;
                int d = (t >> (7 - j)) & 1;
                int idx = (a << 3) | (b << 2) | (cbit << 1) | d;
                if (!PATTERN_ALLOWED[idx]) { ok = 0; break; }
            }
            if (ok) {
                NEXTS[s][c++] = (unsigned short)t;
            }
        }
        COUNT[s] = c;
    }
    INITIALIZED = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based:  a(1) corresponds to a 2×9 array.  */
    init_adjacency();

    seq_t dp[512];
    seq_t dp2[512];
    int s, ns, k, i;

    /* Every possible first row is allowed: start with count 1 for each. */
    for (i = 0; i < 512; ++i) dp[i] = (seq_t)1;

    /* Each transition corresponds to adding a new row.
     * There are exactly n transitions for an (n+1)×9 array. */
    for (i = 0; i < (int)n; ++i) {
        for (s = 0; s < 512; ++s) dp2[s] = (seq_t)0;
        for (s = 0; s < 512; ++s) {
            if (dp[s]) {
                unsigned char c = COUNT[s];
                const unsigned short *next = NEXTS[s];
                for (k = 0; k < c; ++k) {
                    ns = next[k];
                    dp2[ns] += dp[s];
                }
            }
        }
        for (s = 0; s < 512; ++s) dp[s] = dp2[s];
    }

    /* Sum counts over all final rows. */
    seq_t result = (seq_t)0;
    for (i = 0; i < 512; ++i) result += dp[i];
    return result;
}
