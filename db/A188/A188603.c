/*
 * A188603  Number of nX5 binary arrays without the pattern 1 1 0 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding the pattern 1 1 0
 *            diagonally, vertically, antidiagonally or horizontally.
 *            Algorithm: dynamic programming over the last two rows. For n = 1
 *            return the number of single rows with no horizontal 110 (20).
 *            For n ≥ 2 start with all pairs of valid rows; for ea...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file uses types seq_t and idx_t supplied by the build. */
#include <stdbool.h>
#include <stddef.h>

#define BITS        5
#define ROWMASKS   (1 << BITS)
#define STATECNT   (ROWMASKS * ROWMASKS)

static bool hvalid[ROWMASKS] = {0};
static bool hvalid_inited = false;
static seq_t validRowsCount = 0;   /* number of rows with no horizontal 110 */

/* Helper to test horizontal 110 in a row mask. */
static bool check_horizontal(uint32_t m)
{
    for (int c = 0; c <= BITS - 3; ++c) {
        if (((m >> c) & 1) && ((m >> (c + 1)) & 1) && !((m >> (c + 2)) & 1))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* initialise arrays of row validity once */
    if (!hvalid_inited) {
        validRowsCount = 0;
        for (int m = 0; m < ROWMASKS; ++m) {
            bool ok = check_horizontal(m);
            hvalid[m] = ok;
            if (ok)
                validRowsCount = validRowsCount + 1;
        }
        hvalid_inited = true;
    }

    if (n == 0)   /* not part of the sequence, but keep graceful */
        return 0;

    if (n == 1)   /* only a single row */
        return validRowsCount;

    /* dp[pp][p] holds the number of ways to reach a state where
       the last two rows are pp (row r-2) and p (row r-1).  We store
       it in a flat array of size STATECNT. */
    static seq_t dp[STATECNT];
    static seq_t next[STATECNT];

    /* initialise dp for all valid pairs of rows (no horizontal 110) */
    for (size_t i = 0; i < STATECNT; ++i)
        dp[i] = 0;

    for (int m0 = 0; m0 < ROWMASKS; ++m0) {
        if (!hvalid[m0]) continue;
        for (int m1 = 0; m1 < ROWMASKS; ++m1) {
            if (!hvalid[m1]) continue;
            size_t idx = ((size_t)m0) * ROWMASKS + m1;
            dp[idx] = dp[idx] + 1;   /* each pair contributes one */
        }
    }

    /* add rows 2 .. n-1 (zero‑based).  For each step we consume
       the current dp and build next, then swap. */
    for (idx_t r = 2; r < n; ++r) {
        /* clear next */
        for (size_t i = 0; i < STATECNT; ++i)
            next[i] = 0;

        for (int mpp = 0; mpp < ROWMASKS; ++mpp) {
            for (int mp = 0; mp < ROWMASKS; ++mp) {
                size_t curIdx = ((size_t)mpp) * ROWMASKS + mp;
                seq_t count = dp[curIdx];
                if (count == 0) continue;

                for (int mc = 0; mc < ROWMASKS; ++mc) {
                    if (!hvalid[mc]) continue;

                    /* vertical 110 with rows mpp, mp, mc */
                    bool bad = false;
                    for (int c = 0; c < BITS; ++c) {
                        if (((mpp >> c) & 1) && ((mp >> c) & 1) && !((mc >> c) & 1)) {
                            bad = true;
                            break;
                        }
                    }
                    if (bad) continue;

                    /* diagonal down-right 110 */
                    for (int c = 0; c <= BITS - 3; ++c) {
                        if (((mpp >> c) & 1) && ((mp >> (c + 1)) & 1) && !((mc >> (c + 2)) & 1)) {
                            bad = true;
                            break;
                        }
                    }
                    if (bad) continue;

                    /* diagonal down-left 110 */
                    for (int c = 2; c < BITS; ++c) {
                        if (((mpp >> c) & 1) && ((mp >> (c - 1)) & 1) && !((mc >> (c - 2)) & 1)) {
                            bad = true;
                            break;
                        }
                    }
                    if (bad) continue;

                    /* valid transition */
                    size_t nextIdx = ((size_t)mp) * ROWMASKS + mc;
                    next[nextIdx] = next[nextIdx] + count;
                }
            }
        }

        /* move next to dp */
        for (size_t i = 0; i < STATECNT; ++i)
            dp[i] = next[i];
    }

    /* sum all terminal states */
    seq_t total = 0;
    for (size_t i = 0; i < STATECNT; ++i)
        total = total + dp[i];

    return total;
}
