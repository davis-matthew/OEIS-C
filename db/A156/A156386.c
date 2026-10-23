/*
 * A156386  Number of 5X5 arrays of squares of integers, symmetric about both
 *          diagonal and antidiagonal, with all rows summing to n.
 * https://oeis.org/A156386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×5 arrays of squares of integers, * symmetric
 *            about both main diagonal and antidiagonal, * with all rows
 *            summing to n
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* defined types: seq_t, idx_t */
seq_t a(idx_t n) {
    /* compute all square numbers <= n */
    size_t max_i = (size_t)(sqrt((double)n) + 1.0);
    idx_t *sqs = (idx_t*)malloc(max_i * sizeof(idx_t));
    if (!sqs) return (seq_t)0;
    size_t sq_cnt = 0;
    for (idx_t i = 0;; ++i) {
        idx_t v = i * i;
        if (v > n) break;
        sqs[sq_cnt++] = v;
    }

    /* boolean array marking which numbers up to n are squares */
    size_t is_sq_size = (size_t)n + 1;
    unsigned char *is_sq = (unsigned char*)calloc(is_sq_size, sizeof(unsigned char));
    if (!is_sq) { free(sqs); return (seq_t)0; }
    for (size_t i = 0; i < sq_cnt; ++i)
        is_sq[sqs[i]] = 1;

    /* precompute counts of ordered pairs of squares with a given sum */
    size_t max_sum = 2 * n;
    seq_t *ways = (seq_t*)calloc(max_sum + 1, sizeof(seq_t));
    if (!ways) { free(sqs); free(is_sq); return (seq_t)0; }
    for (size_t i = 0; i < sq_cnt; ++i) {
        for (size_t j = 0; j < sq_cnt; ++j) {
            idx_t s = sqs[i] + sqs[j];
            ways[(size_t)s] += (seq_t)1;
        }
    }

    seq_t ans = 0;

    /* main enumeration using derived linear equations */
    for (size_t i6 = 0; i6 < sq_cnt; ++i6) {
        for (size_t i8 = 0; i8 < sq_cnt; ++i8) {
            idx_t x6 = sqs[i6];
            idx_t x8 = sqs[i8];
            idx_t sum_hex = 2 * x6 + 2 * x8;
            if (sum_hex > n) continue;
            idx_t temp = n - sum_hex;
            if (!is_sq[temp]) continue;              /* x4 must be a square (center) */

            idx_t S1 = n - x6;                       /* from row 0 equation */
            idx_t S2 = n - x8;                       /* from row 1 equation */
            idx_t Tmax = (S1 < S2) ? S1 : S2;

            for (idx_t T = 0; T <= Tmax; ++T) {
                seq_t w_cd = ways[(size_t)T];        /* ordered pairs (x5,x7) with sum T */
                if (!w_cd) continue;
                idx_t rem1 = S1 - T;                 /* remainder for (x0,x1) */
                idx_t rem2 = S2 - T;                 /* remainder for (x2,x3) */
                if (rem1 > max_sum || rem2 > max_sum) continue;
                seq_t w_ab = ways[(size_t)rem1];
                seq_t w_ef = ways[(size_t)rem2];
                if (!w_ab || !w_ef) continue;
                ans += w_cd * w_ab * w_ef;
            }
        }
    }

    free(sqs);
    free(is_sq);
    free(ways);
    return ans;
}
