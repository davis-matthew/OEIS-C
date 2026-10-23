/*
 * A161062  Number of partitions of n into powers of two where every part
 *          appears at least 13 times.
 * https://oeis.org/A161062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 139, u16 n <= 445, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of partitions of n into powers of two,
   where every part appears either 0 or at least 13 times. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate and initialise DP arrays */
    idx_t size = n + 1U;
    seq_t *dp  = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    seq_t *dp2 = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    if (!dp || !dp2) {               /* allocation failure */
        free(dp); free(dp2);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;                 /* empty partition */

    for (idx_t v = 1; v <= n; v <<= 1)          /* powers of two */
    {
        /* For each residue class modulo v */
        for (idx_t r = 0; r < v && r <= n; ++r)
        {
            seq_t sum13 = (seq_t)0;              /* sum of old[0..k-13] */
            seq_t buf[13] = {0};                 /* circular buffer of old values */

            for (idx_t j = 0;; ++j)
            {
                idx_t idx = r + j * v;
                if (idx > n) break;

                seq_t old_val = dp[idx];
                buf[j % 13U] = old_val;

                seq_t val = old_val;
                if (j >= 13U) val += sum13;      /* add contributions from >=13 copies */

                dp2[idx] = val;

                if (j >= 12U) {
                    /* sum13 for next j is previous sum13 + old[j-12] */
                    size_t b = (j - 12U) % 13U;
                    sum13 += buf[b];
                }
            }
        }
        /* Swap dp and dp2 for next coin value */
        seq_t *tmp  = dp;
        dp          = dp2;
        dp2         = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(dp2);
    return result;
}
