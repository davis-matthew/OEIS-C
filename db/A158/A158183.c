/*
 * A158183  Number of nondecreasing integer sequences of length 49 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{49} P(n,k) * P_atmost(n,49-k) where P(n,k) is
 *            the number of partitions of n into exactly k positive parts and
 *            P_atmost(n,m) = sum_{j=0}^{m} P(n,j). For n <= 24 this reduces
 *            to P(n,0..n)^2 = A000041(n)^2
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 131, u128 n <= 191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------
   DP tables
   dp[n][k]  - partitions of n into exactly k parts
   pref[n][m] - partitions of n into at most m parts
   ------------------------------------------------------------- */

#define MAX_PARTS 49

static seq_t *dp = NULL;         /* (n+1) * (MAX_PARTS+1) */
static seq_t *pref = NULL;      /* same size as dp */
static idx_t  Nmax = 0;          /* largest n for which tables are computed */

/* ensure tables up to index n (inclusive) */
static void ensure_up_to(idx_t n)
{
    if (n <= Nmax)
        return;

    const size_t old_rows = (size_t)(Nmax + 1);
    const size_t new_rows = (size_t)(n + 1);
    const size_t new_size = new_rows * (MAX_PARTS + 1);

    seq_t *new_dp   = (seq_t *)malloc(new_size * sizeof(seq_t));
    seq_t *new_pref = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_dp || !new_pref)
        abort();

    /* zero new memory */
    memset(new_dp,   0, new_size * sizeof(seq_t));
    memset(new_pref, 0, new_size * sizeof(seq_t));

    /* copy old data */
    if (dp && pref) {
        size_t old_size = old_rows * (MAX_PARTS + 1);
        for (size_t i = 0; i < old_size; ++i) {
            new_dp[i]   = dp[i];
            new_pref[i] = pref[i];
        }
        free(dp);
        free(pref);
    }

    dp   = new_dp;
    pref = new_pref;

    /* initialise row 0 if it did not exist before */
    if (Nmax == 0) {
        dp[0] = (seq_t)1;           /* P(0,0) = 1 */
        pref[0] = (seq_t)1;         /* P_atmost(0,0) = 1 */
        for (idx_t m = 1; m <= MAX_PARTS; ++m)
            pref[m] = pref[m - 1];   /* all pref[0][m] = 1 */
    }

    /* compute new rows from Nmax+1 to n */
    for (idx_t i = Nmax + 1; i <= n; ++i) {
        idx_t base = i * (MAX_PARTS + 1);

        /* dp[i][0] = 0 for i > 0 */
        dp[base + 0] = (seq_t)0;

        for (idx_t k = 1; k <= MAX_PARTS; ++k) {
            seq_t val = dp[(i - 1) * (MAX_PARTS + 1) + (k - 1)];
            if (i >= k)
                val += dp[(i - k) * (MAX_PARTS + 1) + k];
            dp[base + k] = val;
        }

        /* pref row: cumulative sum of dp row */
        pref[base + 0] = dp[base + 0];
        for (idx_t m = 1; m <= MAX_PARTS; ++m) {
            pref[base + m] = pref[base + m - 1] + dp[base + m];
        }
    }

    Nmax = n;
}

/* -------------------------------------------------------------
   Public sequence function
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)            /* offset not defined, return 0 */
        return (seq_t)0;

    ensure_up_to(n);

    seq_t result = (seq_t)0;
    idx_t base = n * (MAX_PARTS + 1);

    for (idx_t k = 0; k <= MAX_PARTS; ++k) {
        seq_t dk = dp[base + k];
        if (dk == (seq_t)0)
            continue;
        idx_t rem = MAX_PARTS - k;
        seq_t of = pref[base + rem];
        result += dk * of;
    }

    return result;
}
