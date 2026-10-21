/*
 * A230740  O.g.f.: Sum_{n>=0} x^n * Product_{k=1..n} (k*(k+1)/2 + x) / (1 +
 *          k*(k+1)/2*x).
 * https://oeis.org/A230740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Sum_{m>=0} x^m * Product_{k=1..m}
 *            (k*(k+1)/2 + x) / (1 + k*(k+1)/2*x)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute OEIS sequence A230740 */
seq_t a(idx_t n)
{
    /* Work only for non‑negative n, n stored in idx_t (uint64_t) */
    size_t size = (size_t)n + 1;

    /* Allocate working arrays */
    seq_t *a_pos = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *a_neg = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *r_pos = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *r_neg = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *new_pos = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *new_neg = (seq_t*)calloc(size, sizeof(seq_t));

    if (!a_pos || !a_neg || !r_pos || !r_neg || !new_pos || !new_neg)
        return 0;   /* allocation failed; return zero as a conservative value */

    /* Base case r_0 = 1 */
    r_pos[0] = 1;

    for (idx_t m = 0; m <= n; ++m) {
        /* Add contribution of the current r to a */
        size_t add_max = (size_t)(n - m);
        for (size_t j = 0; j <= add_max; ++j) {
            a_pos[m + j] += r_pos[j];
            a_neg[m + j] += r_neg[j];
        }
        if (m == n) break;   /* finished the needed m values */

        /* Compute triangular number T_{m+1} = (m+1)*(m+2)/2 */
        seq_t T = (seq_t)(m + 1) * (seq_t)(m + 2);
        T /= 2;

        size_t L = (size_t)(n - (m + 1));
        /* Compute next r (new_pos, new_neg) */
        seq_t prev_pos = 0;
        seq_t prev_neg = 0;
        for (size_t i = 0; i <= L; ++i) {
            /* tmp = r * (T + x)  */
            seq_t tmp_pos = r_pos[i] * T;      /* r[i] * T      */
            seq_t tmp_neg = r_neg[i] * T;      /* r[i] * T      */
            if (i > 0) {
                tmp_pos += r_pos[i - 1];
                tmp_neg += r_neg[i - 1];
            }

            /* t1 = T * new_prev   (for subtraction) */
            seq_t t1_pos = prev_pos * T;
            seq_t t1_neg = prev_neg * T;

            /* new = tmp - t1  expressed as (pos,neg) pair */
            new_pos[i] = tmp_pos + t1_neg;
            new_neg[i] = tmp_neg + t1_pos;

            prev_pos = new_pos[i];
            prev_neg = new_neg[i];
        }

        /* Copy new r into r arrays for next iteration */
        for (size_t i = 0; i <= L; ++i) {
            r_pos[i] = new_pos[i];
            r_neg[i] = new_neg[i];
        }
    }

    seq_t result = a_pos[n] - a_neg[n];

    /* Free allocated memory */
    free(a_pos); free(a_neg);
    free(r_pos); free(r_neg);
    free(new_pos); free(new_neg);

    return result;
}
