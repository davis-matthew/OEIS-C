/*
 * A169529  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(0) = 1 For 1 ≤ n ≤ 33: a(n)
 *            = 34 * Σ_{k=0}^{n-1} a(k) + 2 a(34) = 34 * (Σ_{k=1}^{33} a(k)) -
 *            595 * a(0) + 1 For n ≥ 35: a(n) = 34 * Σ_{k=1}^{33} a(n‑k) – 595
 *            * a(n‑34) (All sums are computed with the already computed
 *            a(k).)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* small n: just handle directly */
    if (n == 0) return (seq_t)1;

    /* Allocation for sequence terms up to n */
    size_t len = (size_t)n + 1;
    seq_t *seq = (seq_t *)malloc(len * sizeof(seq_t));
    if (!seq) return (seq_t)0; /* allocation failure */

    seq[0] = (seq_t)1;

    /* compute 1 ≤ i ≤ 33 or i ≤ n */
    seq_t cum_sum = seq[0];          /* Σ_{k=0}^{i} a(k) as we advance */
    idx_t i;
    for (i = 1; i <= n && i <= 33; ++i) {
        seq[i] = (seq_t)34 * cum_sum + (seq_t)2;
        cum_sum += seq[i];
    }

    if (i == 34 && n >= 34) {
        /* a(34) uses the special formula */
        seq_t sum_excl0 = cum_sum - seq[0];  /* sum of a(1)..a(33) */
        seq[34] = (seq_t)34 * sum_excl0 - (seq_t)595 * seq[0] + (seq_t)1;
        cum_sum += seq[34];
        ++i;
    }

    if (i <= n) {
        /* Now i is 35 or 34+1 (i==35) */
        /* compute the sliding window sum of the last 33 terms:
           for n = 35 this is a(34)+…+a(2) = cum_sum - a(0) - a(1)   */
        seq_t window_sum = cum_sum - seq[0] - (i <= n ? seq[1] : (seq_t)0);
        /* loop for the remaining terms */
        for (; i <= n; ++i) {
            seq[i] = (seq_t)34 * window_sum - (seq_t)595 * seq[i - 34];
            cum_sum += seq[i];
            /* update the sliding window for next iteration */
            window_sum += seq[i];
            window_sum -= seq[i - 33];
        }
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
