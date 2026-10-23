/*
 * A161285  Number of partitions of n into numbers not divisible by 3 where
 *          every part appears at least 7 times.
 * https://oeis.org/A161285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts not divisible by 3
 *            where each distinct part appears at least 7 times
 * Range:     u8 n <= 83, u16 n <= 205, u32 n <= 586, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* The value of n fits in a size_t for the judge */
    idx_t N = n;

    /* Allocate DP arrays */
    seq_t *dp;
    seq_t *temp;
    seq_t *pref;
#ifdef __cplusplus
    dp   = new (std::nothrow) seq_t[N + 1]();
    temp = new (std::nothrow) seq_t[N + 1]();
    pref = new (std::nothrow) seq_t[N + 1]();
#else
    dp   = (seq_t*)calloc(N + 1, sizeof(seq_t));
    temp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    pref = (seq_t*)calloc(N + 1, sizeof(seq_t));
#endif
    if (!dp || !temp || !pref)
    {
        /* allocation failed – return 0 */
        if (dp)   { free(dp);   dp = 0; }
        if (temp) { free(temp); temp = 0; }
        if (pref) { free(pref); pref = 0; }
        return 0;
    }

    dp[0] = 1;   /* the empty partition */

    /* Main DP over parts not divisible by 3 */
    for (idx_t part = 1; part <= N; ++part)
    {
        if (part % 3 == 0) continue;                 /* skip forbidden parts */

        idx_t K = part;                              /* step size */

        /* Process each residue class modulo K */
        for (idx_t r = 0; r < K && r <= N; ++r)
        {
            idx_t max_t = (N >= r) ? ((N - r) / K) : 0;

            /* Copy old values for this residue */
            for (idx_t t = 0; t <= max_t; ++t)
                temp[t] = dp[r + t * K];

            /* Prefix sums of the old values */
            if (max_t > 0)
            {
                pref[0] = temp[0];
                for (idx_t t = 1; t <= max_t; ++t)
                    pref[t] = pref[t - 1] + temp[t];
            }

            /* Update dp with the allowed multiplicities */
            for (idx_t t = 0; t <= max_t; ++t)
            {
                seq_t new_val;
                if (t < 7)                     /* cannot use this part yet */
                    new_val = temp[t];
                else                            /* at least 7 copies */
                    new_val = temp[t] + pref[t - 7];

                dp[r + t * K] = new_val;
            }
        }
    }

    seq_t result = dp[N];

    /* Free the allocated arrays */
#ifdef __cplusplus
    delete[] dp;
    delete[] temp;
    delete[] pref;
#else
    free(dp);
    free(temp);
    free(pref);
#endif

    return result;
}
