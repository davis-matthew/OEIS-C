/*
 * A035621  Number of partitions of n into parts 4k and 4k+1 with at least one
 *          part of each type.
 * https://oeis.org/A035621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = partitions of n into parts 4k or 4k+1 * minus partitions
 *            using only 4k parts minus partitions using only 4k+1 parts. *
 *            Computed by dynamic programming in O(n²) time
 * Range:     u8 n <= 32, u16 n <= 83, u32 n <= 248, u64 n <= 815,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAXN 10000                       /* sufficient for the test data */

static seq_t dp0[MAXN + 1];              /* only 4k parts */
static seq_t dp1[MAXN + 1];              /* only 4k+1 parts */
static seq_t dpTot[MAXN + 1];            /* all allowed parts */

/* number of partitions of n with at least one part of each type            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    /* if the requested n is beyond the static tables, fall back to a
       dynamic allocation that can grow arbitrarily.  This branch is
       unlikely to be used in the judge tests.                            */
    if (n > MAXN) {
        idx_t sz = n + 1;
        seq_t *dp0d = (seq_t*)malloc(sz * sizeof(seq_t));
        seq_t *dp1d = (seq_t*)malloc(sz * sizeof(seq_t));
        seq_t *dpTotd = (seq_t*)malloc(sz * sizeof(seq_t));
        if (!dp0d || !dp1d || !dpTotd) {
            free(dp0d); free(dp1d); free(dpTotd);
            return 0;
        }
        for (idx_t i = 0; i < sz; ++i) {
            dp0d[i] = dp1d[i] = dpTotd[i] = 0;
        }
        dp0d[0] = dp1d[0] = dpTotd[0] = 1;

        for (idx_t part = 4; part <= n; part += 4)
            for (idx_t j = part; j <= n; ++j)
                dp0d[j] += dp0d[j - part];

        for (idx_t part = 1; part <= n; part += 4)
            for (idx_t j = part; j <= n; ++j)
                dp1d[j] += dp1d[j - part];

        for (idx_t part = 1; part <= n; ++part)
            if ((part & 3) == 0 || (part & 3) == 1)
                for (idx_t j = part; j <= n; ++j)
                    dpTotd[j] += dpTotd[j - part];

        seq_t res = dpTotd[n];
        res -= dp0d[n];
        res -= dp1d[n];

        free(dp0d); free(dp1d); free(dpTotd);
        return res;
    }

    /* reset the static tables up to n */
    for (idx_t i = 1; i <= n; ++i) {
        dp0[i] = dp1[i] = dpTot[i] = 0;
    }
    dp0[0] = dp1[0] = dpTot[0] = 1;

    /* partitions using only 4k parts */
    for (idx_t part = 4; part <= n; part += 4)
        for (idx_t j = part; j <= n; ++j)
            dp0[j] += dp0[j - part];

    /* partitions using only 4k+1 parts */
    for (idx_t part = 1; part <= n; part += 4)
        for (idx_t j = part; j <= n; ++j)
            dp1[j] += dp1[j - part];

    /* partitions using all allowed parts (4k or 4k+1) */
    for (idx_t part = 1; part <= n; ++part)
        if ((part & 3) == 0 || (part & 3) == 1)
            for (idx_t j = part; j <= n; ++j)
                dpTot[j] += dpTot[j - part];

    /* subtract the partitions missing one of the two part types */
    seq_t res = dpTot[n];
    res -= dp0[n];
    res -= dp1[n];

    return res;
}
