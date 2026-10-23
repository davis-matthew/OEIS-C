/*
 * A170431  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 38 for 1 ≤ n ≤ 43: a(n) = 2 + 36 · Σ_{k=0}^{n-1}
 *            a(k) a(44) = 1 + 36 · Σ_{k=1}^{43} a(k) – 666 · a(0) for n ≥ 45:
 *            a(n) = 36 · Σ_{k=1}^{43} a(n-k) – 666 · a(n-44) (All sums are
 *            ordinary integer sums.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    /* Handle the trivial small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)38;

    /* Allocate space for terms up to max(n,44). */
    size_t size = (n >= 44 ? (size_t)n + 1 : (size_t)45);
    seq_t *terms = (seq_t*)malloc(size * sizeof(seq_t));
    if (!terms) return (seq_t)0;   /* allocation failed, return 0 as fallback */

    /* Compute a(0)..a(44). */
    terms[0] = (seq_t)1;
    seq_t cum_sum = terms[0];           /* Σ_{k=0}^{i-1} a(k) seen so far */

    /* 1 ≤ i ≤ 43: a(i) = 2 + 36 * cum_sum. */
    idx_t i;
    for (i = 1; i <= 43 && i <= n; ++i) {
        terms[i] = (seq_t)2 + (seq_t)36 * cum_sum;
        cum_sum += terms[i];
    }
    if (n <= 43) {                      /* answer already in the array */
        seq_t result = terms[n];
        free(terms);
        return result;
    }

    /* Compute a(44). */
    seq_t sum1to43 = cum_sum - terms[0];                  /* a(1)+...+a(43) */
    terms[44] = (seq_t)1 + (seq_t)36 * sum1to43 - (seq_t)666 * terms[0];
    cum_sum += terms[44];                                /* Σ a(0..44) */

    if (n == 44) {                       /* answer is a(44) */
        seq_t result = terms[44];
        free(terms);
        return result;
    }

    /* For n ≥ 45 we use the recurrence:
        a(i) = 36 * Σ_{k=1}^{43} a(i-k) – 666 * a(i-44)      */
    seq_t sum_44 = cum_sum - terms[0];      /* Σ_{k=1}^{44} a(k) */

    for (i = 45; i <= n; ++i) {
        idx_t prev = i - 44;                /* a(i-44) */
        seq_t sum_last_43 = sum_44 - terms[prev];
        seq_t val = (seq_t)36 * sum_last_43 - (seq_t)666 * terms[prev];
        terms[i] = val;

        /* Update the sliding window sum of the last 44 terms. */
        sum_44 += val - terms[prev];
    }

    seq_t result = terms[n];
    free(terms);
    return result;
}
