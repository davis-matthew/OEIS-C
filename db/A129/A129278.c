/*
 * A129278  Column 2 of triangle A129276; a(n) is the coefficient of q^(2n+2)
 *          in the squared q-factorial of n+2.
 * https://oeis.org/A129278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^(2n+2) in ∏_{i=1..n+2} ((1‑q^i)/(1‑q))^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* target degree of the required coefficient   */
    idx_t E = 2 * n + 2;          /* exponent 2n+2                           */
    size_t size = (size_t)E + 1;  /* array length needed                       */

    /* allocate working arrays */
    seq_t *dp   = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *buf  = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *pref0 = (seq_t *)calloc(size + 1, sizeof(seq_t)); /* prefix sums of dp   */
    seq_t *pref1 = (seq_t *)calloc(size + 1, sizeof(seq_t)); /* prefix of k*dp       */

    if (!dp || !buf || !pref0 || !pref1) {
        /* allocation failed; return 0 as fallback (should never happen) */
        free(dp); free(buf); free(pref0); free(pref1);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;  /* start with polynomial = 1 */

    /* iterate over i = 1 .. N (N = n+2) */
    for (idx_t i = 1; i <= n + 2; ++i) {
        /* old degree of product before adding factor i           */
        size_t old_deg = (i <= 2) ? 0 : (size_t)(i - 1) * (size_t)(i - 2);
        if (old_deg > E) old_deg = E;          /* we only keep terms up to E            */

        /* build prefix sums of current polynomial dp[0..old_deg] */
        pref0[0] = (seq_t)0;
        pref1[0] = (seq_t)0;
        for (size_t k = 0; k <= old_deg; ++k) {
            pref0[k + 1] = pref0[k] + dp[k];
            pref1[k + 1] = pref1[k] + seq_mul(dp[k], (seq_t)k);
        }

        /* clear buffer for next polynomial */
        memset(buf, 0, size * sizeof(seq_t));

        /* new degree after adding factor i          */
        size_t new_deg = old_deg + 2 * (size_t)i - 2;
        if (new_deg > E) new_deg = E;

        /* compute new polynomial coefficients for d = 0..new_deg  */
        for (size_t d = 0; d <= new_deg; ++d) {
            seq_t sum = (seq_t)0;

            /* ----- first part: j < i  (j = d-k, k in [d-(i-1), d]) ----- */
            size_t lo1 = (d >= (size_t)(i - 1) ? d - (size_t)(i - 1) : 0);
            size_t hi1 = (d <= old_deg ? d : old_deg);
            if (hi1 >= lo1) {
                seq_t B1 = pref0[hi1 + 1] - pref0[lo1];
                seq_t A1 = pref1[hi1 + 1] - pref1[lo1];
                seq_t part1 = seq_mul((seq_t)(d + 1), B1);
                part1 -= A1;
                sum += part1;
            }

            /* ----- second part: i ≤ j ≤ 2i-2  (j = d-k, k in [d-(2i-2), d-i]) ----- */
            int64_t lo2_s = (int64_t)d - (int64_t)(2 * i - 2);
            if (lo2_s < 0) lo2_s = 0;
            int64_t hi2_s = (int64_t)d - (int64_t)i;
            if (hi2_s < 0) hi2_s = -1;                     /* make empty if negative   */
            if (hi2_s > (int64_t)old_deg) hi2_s = (int64_t)old_deg;
            if (hi2_s >= lo2_s) {
                size_t lo2 = (size_t)lo2_s;
                size_t hi2 = (size_t)hi2_s;
                seq_t B2 = pref0[hi2 + 1] - pref0[lo2];
                seq_t A2 = pref1[hi2 + 1] - pref1[lo2];

                int64_t const_term = (int64_t)(2 * i - 1) - (int64_t)d;
                seq_t part2;
                if (const_term >= 0) {
                    part2 = seq_mul((seq_t)const_term, B2);
                    part2 += A2;
                } else {
                    seq_t tmp = seq_mul((seq_t)(-const_term), B2);
                    part2 = A2 - tmp;            /* constant negative => subtract  */
                }
                sum += part2;
            }

            buf[d] = sum;
        }

        /* swap dp and buf for next iteration */
        seq_t *tmp = dp;
        dp = buf;
        buf = tmp;
    }

    seq_t result = dp[2 * n + 2];   /* coefficient at exponent 2n+2 */

    /* free temporaries */
    free(dp);  free(buf);
    free(pref0);  free(pref1);

    return result;
}
