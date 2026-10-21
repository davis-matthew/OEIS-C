/*
 * A231039  Number of 3 X n 0..2 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 3 and at least one element with value
 *          (x(i,j)-1) mod 3, and upper left element zero.
 * https://oeis.org/A231039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×n arrays over {0,1,2} with upper‑left cell 0,
 *            such that every element has at least one neighbor with value
 *            (x+1) mod 3 and at least one neighbor with value (x-1) mod 3,
 *            where neighbors are those horizontally, diagonally or
 *            antidiagonally adjacent. The DP below counts such array...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define STATE_COUNT 27   /* 3ⁿ column states (3³ for 3 rows) */

static int digits[STATE_COUNT][3];
static uint32_t nextMask[STATE_COUNT][STATE_COUNT];
static uint32_t startMask[STATE_COUNT];
static bool endValid[STATE_COUNT][STATE_COUNT];
static bool tables_ready = false;

static inline bool column_valid(int curr, int prev, int next,
                                bool hasPrev, bool hasNext)
{
    for (int r = 0; r < 3; ++r) {
        int v     = digits[curr][r];
        int plus  = (v + 1) % 3;
        int minus = (v + 2) % 3;
        bool hplus  = false;
        bool hminus = false;

        /* Left / Right in the same row */
        if (hasPrev) {
            int nv = digits[prev][r];
            if (nv == plus)  hplus  = true;
            if (nv == minus) hminus = true;
        }
        if (hasNext) {
            int nv = digits[next][r];
            if (nv == plus)  hplus  = true;
            if (nv == minus) hminus = true;
        }

        /* Diagonal neighbours (up-left / down-right) */
        if (hasPrev && r > 0) {              /* up‑left */
            int nv = digits[prev][r-1];
            if (nv == plus)  hplus  = true;
            if (nv == minus) hminus = true;
        }
        if (hasNext && r < 2) {              /* down‑right */
            int nv = digits[next][r+1];
            if (nv == plus)  hplus  = true;
            if (nv == minus) hminus = true;
        }

        /* Diagonal neighbours (up-right / down-left) */
        if (hasNext && r > 0) {              /* up‑right */
            int nv = digits[next][r-1];
            if (nv == plus)  hplus  = true;
            if (nv == minus) hminus = true;
        }
        if (hasPrev && r < 2) {              /* down‑left */
            int nv = digits[prev][r+1];
            if (nv == plus)  hplus  = true;
            if (nv == minus) hminus = true;
        }

        /* Vertical neighbours are NOT considered (per problem statement) */

        if (!hplus || !hminus)
            return false;
    }
    return true;
}

static void init_tables(void)
{
    /* digits for each state */
    for (int s = 0; s < STATE_COUNT; ++s) {
        digits[s][0] = (s / 9) % 3;      /* row 0 (top) */
        digits[s][1] = (s / 3) % 3;      /* row 1 (middle) */
        digits[s][2] = s % 3;            /* row 2 (bottom) */
    }

    /* transitions for inner columns */
    for (int p = 0; p < STATE_COUNT; ++p)
        for (int c = 0; c < STATE_COUNT; ++c) {
            uint32_t mask = 0;
            for (int n = 0; n < STATE_COUNT; ++n)
                if (column_valid(c, p, n, true, true))
                    mask |= 1u << n;
            nextMask[p][c] = mask;
        }

    /* start mask: first column has no left neighbour */
    for (int c = 0; c < STATE_COUNT; ++c) {
        uint32_t mask = 0;
        for (int n = 0; n < STATE_COUNT; ++n)
            if (column_valid(c, -1, n, false, true))
                mask |= 1u << n;
        startMask[c] = mask;
    }

    /* end validity: last column has no right neighbour */
    for (int p = 0; p < STATE_COUNT; ++p)
        for (int c = 0; c < STATE_COUNT; ++c)
            endValid[p][c] = column_valid(c, p, -1, true, false);

    tables_ready = true;
}

seq_t a(idx_t n)
{
    if (!tables_ready)
        init_tables();

    if (n == 1)          /* first column alone cannot satisfy the rule */
        return (seq_t)0;

    /* dp[prev][curr] - number of prefixes ending with columns prev and curr
       where all columns up to prev are already valid. */
    seq_t dp[STATE_COUNT][STATE_COUNT];
    seq_t nf[STATE_COUNT][STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i)
        for (int j = 0; j < STATE_COUNT; ++j)
            dp[i][j] = (seq_t)0;

    /* initialise for columns 0 and 1 */
    for (int c0 = 0; c0 < STATE_COUNT; ++c0) {
        if (digits[c0][0] != 0)   /* upper-left fixed to 0 */
            continue;
        uint32_t mask = startMask[c0];
        while (mask) {
            int c1 = __builtin_ctz(mask);
            mask &= mask - 1;
            dp[c0][c1] = (seq_t)1;
        }
    }

    /* iterative DP for columns 2 .. n-1 (0‑based indexing) */
    for (idx_t col = 2; col < n; ++col) {
        for (int i = 0; i < STATE_COUNT; ++i)
            for (int j = 0; j < STATE_COUNT; ++j)
                nf[i][j] = (seq_t)0;

        for (int p = 0; p < STATE_COUNT; ++p) {
            for (int c = 0; c < STATE_COUNT; ++c) {
                seq_t cnt = dp[p][c];
                if (!cnt)
                    continue;
                uint32_t mask = nextMask[p][c];
                while (mask) {
                    int nxt = __builtin_ctz(mask);
                    mask &= mask - 1;
                    nf[c][nxt] += cnt;
                }
            }
        }
        /* move nf → dp for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i)
            for (int j = 0; j < STATE_COUNT; ++j)
                dp[i][j] = nf[i][j];
    }

    /* final column validation */
    seq_t result = (seq_t)0;
    for (int p = 0; p < STATE_COUNT; ++p) {
        for (int c = 0; c < STATE_COUNT; ++c) {
            seq_t cnt = dp[p][c];
            if (!cnt)
                continue;
            if (endValid[p][c])
                result += cnt;
        }
    }
    return result;
}
