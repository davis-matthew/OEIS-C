/*
 * A128597  Column 2 of triangle A128596; a(n) = coefficient of q^(2n+4) in
 *          the q-analog of the even double factorials (2n+4)!! for n>=0.
 * https://oeis.org/A128597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS formula:
 *   a(n) = coefficient of q^(2n+4) in
 *          Π_{j=1}^{n+2} (1 - q^(2j)) / (1 - q)
 *
 * The routine computes this coefficient by dynamic programming.
 * For each factor (1 - q^(2j))/(1 - q) we update the
 * polynomial coefficients using a prefix‑sum trick:
 *
 *   new_k = pref_k - pref_{k-2j}   (where pref_k = Σ_{t=0}^k old_t)
 *
 * This yields an O(n·(2n+4)) algorithm, fast enough for n≈10⁴.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Target exponent and array length. */
    size_t target = 2ULL * (size_t)n + 4ULL;   /* 2n+4 */
    size_t len    = target + 1;               /* indices 0 … 2n+4 */

    /* dp holds coefficients of the product processed so far. */
    seq_t *dp  = (seq_t*)calloc(len, sizeof(seq_t));
    if (!dp) abort();
    dp[0] = 1;                    /* start with polynomial 1 */

    /* Temporary arrays.  They stay the same size for every factor. */
    seq_t *ndp = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *pref = (seq_t*)malloc(len * sizeof(seq_t));
    if (!ndp || !pref) abort();

    /* Process factors (1 - q^(2j))/(1 - q) for j = 1 … n+2. */
    for (idx_t j = 1; j <= n + 2; ++j)
    {
        /* Build prefix sums of current coefficients. */
        pref[0] = dp[0];
        for (size_t k = 1; k <= target; ++k)
            pref[k] = pref[k-1] + dp[k];

        /* Compute new coefficients. */
        size_t shift = 2ULL * (size_t)j;          /* 2j */
        for (size_t k = 0; k <= target; ++k)
        {
            seq_t val = pref[k];
            if (k >= shift) val -= pref[k - shift];
            ndp[k] = val;
        }

        /* Swap dp and ndp for the next iteration. */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    seq_t result = dp[target];

    /* Clean up temporary storage. */
    free(dp);
    free(ndp);
    free(pref);

    return result;
}
