/*
 * A208041  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Number of 5×n binary arrays avoiding 0 0 0 and 0 1 0 horizontally
 * and 0 0 0 and 1 0 1 vertically. */

/* 13 columns satisfy the vertical constraints. */
static int m;                     /* number of admissible columns */
static unsigned char bits[32][5];  /* bits for each admissible column   */
static unsigned short trans[169][32]; /* transitions between ordered pairs */
static unsigned char deg[169];         /* out‑degree of each state         */
static bool initialized = false;

/* Build the admissible column set, the transition table and degrees. */
static void init(void)
{
    int pat, r;
    m = 0;
    for (pat = 0; pat < 32; ++pat) {
        unsigned char col[5];
        for (r = 0; r < 5; ++r)
            col[r] = (pat >> r) & 1U;

        /* check vertical patterns 000 and 101 */
        bool ok = true;
        for (r = 0; ok && r <= 2; ++r) {
            int a = col[r], b = col[r + 1], c = col[r + 2];
            if ((a == 0 && b == 0 && c == 0) ||
                (a == 1 && b == 0 && c == 1))
                ok = false;
        }
        if (!ok)
            continue;

        for (r = 0; r < 5; ++r)
            bits[m][r] = col[r];
        ++m;
    }

    int S = m * m;                /* number of ordered column pairs */

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            int s = i * m + j;
            int cnt = 0;
            for (int k = 0; k < m; ++k) {
                bool ok = true;
                for (r = 0; ok && r < 5; ++r) {
                    int a = bits[i][r];
                    int b = bits[j][r];
                    int c = bits[k][r];
                    if ((a == 0 && b == 0 && c == 0) ||
                        (a == 0 && b == 1 && c == 0))
                        ok = false;
                }
                if (ok)
                    trans[s][cnt++] = j * m + k;   /* destination (j,k) */
            }
            deg[s] = (unsigned char)cnt;
        }
    }

    initialized = true;
}

/* Count arrays of width n.  For n = 1, the answer is the number of
 * admissible columns (13).  For n = 2, it is 13² = 169.  For
 * n ≥ 3 we perform a dynamic‑programming walk over the state graph
 * described above.  The table size is at most 169, so the routine runs
 * in O(n) time and uses negligible memory.  The function works for
 * any seq_t that supports the basic integer operators. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* n < 1 is outside the defined range */

    if (n == 1)
        return (seq_t)13;         /* one column */

    if (!initialized)
        init();

    int S = m * m;

    /* local DP tables */
    seq_t cur[169];
    seq_t nxt[169];

    /* base: all ordered pairs are initially available once */
    for (int s = 0; s < S; ++s)
        cur[s] = (seq_t)1;

    idx_t steps = n - 2;          /* number of transitions to make */

    for (idx_t t = 0; t < steps; ++t) {
        /* zero next table */
        for (int i = 0; i < S; ++i)
            nxt[i] = (seq_t)0;

        /* propagate counts */
        for (int s = 0; s < S; ++s) {
            seq_t v = cur[s];
            if (!v)
                continue;
            int d = deg[s];
            for (int k = 0; k < d; ++k)
                nxt[trans[s][k]] += v;
        }

        /* swap cur and nxt */
        for (int i = 0; i < S; ++i)
            cur[i] = nxt[i];
    }

    /* total ways = sum of all final states */
    seq_t result = (seq_t)0;
    for (int s = 0; s < S; ++s)
        result += cur[s];

    return result;
}
