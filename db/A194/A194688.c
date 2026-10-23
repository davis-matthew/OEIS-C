/*
 * A194688  First differences of A036554 (numbers whose binary representation
 *          ends in an odd number of zeros).
 * https://oeis.org/A194688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A036554(n+1) – A036554(n) where A036554 are the numbers
 *            whose binary representation ends in an odd number of zeros
 * Range:     u8 n <= 42, u16 n <= 138, u32 n <= 138, u64 n <= 138,
 *            u128 n <= 138, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1; return 0 just in case */

    /* Estimate the largest value needed.  Each term of the sequence occupies roughly
       1/3 of the positive integers, so the (n+1)-th term is near 3*(n+1). */
    idx_t max_est = n * (idx_t)4 + 10;     /* extra buffer to be safe */

    /* Find the largest odd exponent t so that 2^t <= max_est. */
    idx_t t = 1;
    while (((idx_t)1 << t) <= max_est) t++;
    if (t % 2 == 0) t--;          /* make t odd */

    /* Number of streams (odd t values). */
    size_t T = (t + 1) / 2;

    /* Allocate arrays for the current values and steps of each stream. */
    seq_t *val  = (seq_t *)malloc(T * sizeof(seq_t));
    seq_t *step = (seq_t *)malloc(T * sizeof(seq_t));
    if (!val || !step) return 0;  /* allocation failed */

    for (size_t i = 0; i < T; ++i) {
        idx_t cur_t = 2 * i + 1;
        val[i]  = ((seq_t)1 << (unsigned)cur_t);
        step[i] = ((seq_t)1 << (unsigned)(cur_t + 1));
    }

    /* Generate the first n+1 terms of A036554. */
    seq_t *terms = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!terms) { free(val); free(step); return 0; }

    idx_t generated = 0;
    while (generated <= n) {          /* need terms[0] … terms[n] */
        /* Find the smallest current value among all streams. */
        size_t min_idx = 0;
        seq_t min_val = val[0];
        for (size_t i = 1; i < T; ++i) {
            if (val[i] < min_val) {
                min_val = val[i];
                min_idx = i;
            }
        }

        terms[generated] = min_val;               /* store the generated term */
        val[min_idx] += step[min_idx];            /* advance that stream */
        ++generated;
    }

    /* The desired difference is terms[n] – terms[n-1]. */
    seq_t result = terms[n] - terms[n - 1];

    /* Clean up. */
    free(terms);
    free(val);
    free(step);

    return result;
}
