/*
 * A237072  Number of (n+1)X(4+1) 0..3 arrays with the upper median of every
 *          2X2 subblock equal.
 * https://oeis.org/A237072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{M=0}^{3} # of (n+1)×5 matrices with entries in
 *            {0,1,2,3} such that every 2×2 block has upper median equal to M.
 *            The algorithm counts, for each M, the number of ways to choose
 *            rows of length 5 whose categories (≺M, =M, ≻M) satisfy the local
 *            constraints. It uses dynamic programming over...
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define PAT 243          /* 3^5 possible row patterns */
#define MAX_IN 250       /* maximum incoming edges per pattern */

/* Pre‑computed data */
static bool initialized = false;
static int    cat[PAT][5];          /* categories of a pattern */
static int    lowcnt[PAT];          /* number of category 0 cells */
static int    highcnt[PAT];         /* number of category 2 cells */

/* Incoming adjacency lists: patterns that can precede j */
static int    inbound[PAT][MAX_IN];
static int    inbound_cnt[PAT];

/* ------------------------------------------------------------------ */
/* Helper: compute pattern information and adjacency once. */
static void init_patterns(void)
{
    if (initialized)
        return;

    /* -------------------------------------------------------------- */
    /* 1. Enumerate all 3^5 patterns: categories per column. */
    int pow3[6];
    pow3[0] = 1;
    for (int i = 1; i <= 5; ++i)
        pow3[i] = pow3[i - 1] * 3;

    for (int idx = 0; idx < PAT; ++idx) {
        int tmp = idx;
        lowcnt[idx] = 0;
        highcnt[idx] = 0;
        for (int col = 0; col < 5; ++col) {
            int cat_val = tmp % 3;      /* 0 = <M, 1 = =M, 2 = >M */
            cat[idx][col] = cat_val;
            if (cat_val == 0)
                ++lowcnt[idx];
            else if (cat_val == 2)
                ++highcnt[idx];
            tmp /= 3;
        }
        inbound_cnt[idx] = 0;           /* will fill later */
    }

    /* -------------------------------------------------------------- */
    /* 2. Build adjacency (independent of M). */
    for (int p = 0; p < PAT; ++p) {
        for (int q = 0; q < PAT; ++q) {
            bool ok = true;
            for (int j = 0; j < 4 && ok; ++j) {
                int a = cat[p][j];
                int b = cat[p][j + 1];
                int c = cat[q][j];
                int d = cat[q][j + 1];

                int cnt2 = 0, cnt1 = 0;
                if (a == 2) ++cnt2; else if (a == 1) ++cnt1;
                if (b == 2) ++cnt2; else if (b == 1) ++cnt1;
                if (c == 2) ++cnt2; else if (c == 1) ++cnt1;
                if (d == 2) ++cnt2; else if (d == 1) ++cnt1;

                if (cnt2 > 1 || (cnt1 + cnt2) < 2)
                    ok = false;      /* second largest would not be 1 */
            }
            if (ok) {
                inbound[q][ inbound_cnt[q]++ ] = p;   /* p allowed before q */
            }
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Main function required by the build.                                */
seq_t a(idx_t n)
{
    init_patterns();

    /* number of rows in the matrix (n+1) */
    const idx_t rows = n + 1;
    seq_t total = 0;

    /* temporary buffers for dynamic programming */
    static seq_t dp[PAT];
    static seq_t newdp[PAT];

    /* Process each possible equal median M = 0..3 */
    for (int M = 0; M <= 3; ++M) {
        /* row_counts[pat] = M^(lowcnt) * (3-M)^(highcnt) */
        static seq_t rowcnt[PAT];
        for (int pat = 0; pat < PAT; ++pat) {
            seq_t val = 1;
            /* multiply by M for each low cell */
            for (int k = 0; k < lowcnt[pat]; ++k)
                val *= (seq_t)M;
            /* multiply by (3-M) for each high cell */
            for (int k = 0; k < highcnt[pat]; ++k)
                val *= (seq_t)(3 - M);
            rowcnt[pat] = val;
        }

        /* initialise DP with the first row */
        for (int pat = 0; pat < PAT; ++pat)
            dp[pat] = rowcnt[pat];

        /* iterate over the remaining rows */
        for (idx_t r = 1; r < rows; ++r) {
            for (int pat = 0; pat < PAT; ++pat) {
                seq_t sum = 0;
                for (int k = 0; k < inbound_cnt[pat]; ++k) {
                    int prev = inbound[pat][k];
                    sum += dp[prev];
                }
                newdp[pat] = sum * rowcnt[pat];
            }
            /* copy back to dp for next iteration */
            for (int pat = 0; pat < PAT; ++pat)
                dp[pat] = newdp[pat];
        }

        /* add all end states for this M */
        seq_t subtotal = 0;
        for (int pat = 0; pat < PAT; ++pat)
            subtotal += dp[pat];
        total += subtotal;
    }

    return total;
}
