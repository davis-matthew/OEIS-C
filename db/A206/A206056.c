/*
 * A206056  Number of (n+1)X2 0..3 arrays with every 2X3 or 3X2 subblock
 *          having exactly two clockwise edge increases.
 * https://oeis.org/A206056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A206056: the number of (n+1)×2 arrays with entries 0..3 such that
 * every 3×2 subblock has exactly two clockwise edge increases.
 * The implementation uses dynamic programming over rows.
 * Initial values: a(1)=256, a(2)=2184, a(3)=19832, … (computed automatically).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility to extract the two digits of a row (encoded as 0..15) */
static inline void row_digits(int row, int *left, int *right)
{
    *left  = row & 3;          /* low 2 bits */
    *right = (row >> 2) & 3;   /* next 2 bits */
}

/* Checks whether a 3×2 block defined by rows r1,r2,r3 satisfies the
   “exactly two clockwise edge increases” condition. */
static inline bool validBlock(int r1, int r2, int r3)
{
    int a, b, c, d, e, f;
    row_digits(r1, &a, &b);   // top row
    row_digits(r2, &c, &d);   // middle row
    row_digits(r3, &e, &f);   // bottom row

    int inc = 0;
    inc += (b > a);   // a → b
    inc += (d > b);   // b → d
    inc += (f > d);   // d → f
    inc += (e > f);   // f → e
    inc += (c > e);   // e → c
    inc += (a > c);   // c → a
    return inc == 2;
}

/* Pre‑computed transitions: for each pair (prevprev, prev) the list of
   allowed next rows that keep the 3×2 block valid. */
static bool initialized = false;
static uint8_t  transCount[256];
static uint8_t  transVals[256][16];   /* maximum 16 transitions per state */

static void init_transitions(void)
{
    if (initialized) return;

    for (int ppv = 0; ppv < 16; ++ppv)            /* prevprev */
    for (int pv  = 0; pv  < 16; ++pv)   /* prev */
    {
        int idx = ppv * 16 + pv;
        int cnt = 0;
        for (int nv = 0; nv < 16; ++nv)             /* next */
            if (validBlock(ppv, pv, nv))
                transVals[idx][cnt++] = nv;
        transCount[idx] = (uint8_t)cnt;
    }
    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_transitions();                     /* ensure transitions ready */

    const idx_t rows = n + 1;               /* total number of rows */

    /* dp[cur][prev][curr] = number of arrays ending with rows (prev,curr) */
    static seq_t dp[2][16][16];
    int cur = 0, nxt = 1;

    /* Initialize for the base case: 2 rows (n=1) */
    for (int r1 = 0; r1 < 16; ++r1)
        for (int r2 = 0; r2 < 16; ++r2)
            dp[cur][r1][r2] = (seq_t)1;

    /* Build arrays with additional rows */
    for (idx_t r = 3; r <= rows; ++r)
    {
        /* zero-out next layer */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[nxt][i][j] = (seq_t)0;

        /* transition */
        for (int prevprev = 0; prevprev < 16; ++prevprev)
        for (int prev     = 0; prev        < 16; ++prev)
        {
            seq_t val = dp[cur][prevprev][prev];
            if (val == (seq_t)0) continue;

            int idx = prevprev * 16 + prev;
            uint8_t cnt = transCount[idx];
            for (uint8_t k = 0; k < cnt; ++k)
            {
                int next = transVals[idx][k];
                dp[nxt][prev][next] += val;
            }
        }
        /* swap layers */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* sum over all end states */
    seq_t total = (seq_t)0;
    for (int prev = 0; prev < 16; ++prev)
    for (int curr = 0; curr < 16; ++curr)
        total += dp[cur][prev][curr];

    return total;
}
