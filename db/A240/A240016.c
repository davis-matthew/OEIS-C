/*
 * A240016  Number of partitions of n, where the difference between the number
 *          of odd parts and the number of even parts is 7.
 * https://oeis.org/A240016
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0} [x^(n-(3k+7))] 1 /
 *            (∏_{j=1..k}(1-x^(2j))·∏_{j=1..k+7}(1-x^(2j)))
 * Range:     u8 n <= 30, u16 n <= 62, u32 n <= 161, u64 n <= 481,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates the sum by computing partition counts of integers
   into parts ≤k using a dynamic programming array of size (T_max+1) per k.
   The convolution for each k is performed when the second row becomes available,
   keeping the memory consumption well below the 2 GB limit. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)7) return 0;

    /* upper bounds for the DP tables */
    const size_t Kmax   = (size_t)(n - 7) / 3;
    const size_t T_max  = (size_t)(n - 7) / 2;   /* maximum t = rest/2 */
    const size_t stride = T_max + 1;

    /* allocate 8 rows for a circular buffer of p(k, t) */
    seq_t *rows[8];
    for (size_t i = 0; i < 8; ++i) {
        rows[i] = (seq_t *)calloc(stride, sizeof(seq_t));
        if (!rows[i]) {
            for (size_t j = 0; j < i; ++j) free(rows[j]);
            return 0;
        }
    }

    /* base case: p(0,0)=1 */
    rows[0][0] = (seq_t)1;

    seq_t ans = (seq_t)0;

    /* iterate over k = 1 .. Kmax+7, computing rows sequentially */
    for (size_t k = 1; k <= Kmax + 7; ++k) {
        size_t r      = k % 8;
        size_t prev_r = (k - 1) % 8;
        seq_t *cur    = rows[r];
        const seq_t *prev = rows[prev_r];

        /* recurrence: cur[t] = prev[t] + cur[t-k] (if t>=k) */
        for (size_t t = 0; t <= T_max; ++t) {
            seq_t val = prev[t];
            if (t >= k) val += cur[t - k];
            cur[t] = val;
        }

        /* when k≥7, we can evaluate the convolution for k-7 */
        if (k >= 7) {
            const size_t km7 = k - 7;
            idx_t base = (idx_t)(3 * km7) + (idx_t)7;
            if (n < base) continue;                           /* rest < 0, skip */
            idx_t rest = n - base;                            /* rest = n - (3(k-7)+7)   */
            if ((rest & (idx_t)1) != 0) continue;                 /* rest must be even */

            const size_t T = (size_t)(rest / 2);                /* t = rest/2 */
            if (T > T_max) continue;

            seq_t *row_km7 = rows[km7 % 8];
            seq_t sum_keta = (seq_t)0;
            for (size_t s = 0; s <= T; ++s) {
                sum_keta += row_km7[s] * cur[T - s];
            }
            ans += sum_keta;
        }
    }

    /* free allocated memory */
    for (size_t i = 0; i < 8; ++i) free(rows[i]);

    return ans;
}
