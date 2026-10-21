/*
 * A086637  Row sums of triangle A086636.
 * https://oeis.org/A086637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n-1) + \sum_{i+j+k = n-1} a(i)a(j)a(k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Allocate storage for a[0..n] and AA[0..n] */
    seq_t *a_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *AA   = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (a_arr == NULL || AA == NULL) {
        /* Allocation failure – return undefined value */
        free(a_arr);
        free(AA);
        return (seq_t)0;
    }

    a_arr[0] = (seq_t)1;
    AA[0]    = a_arr[0] * a_arr[0];   /* a(0)^2 */

    /* Start with Catalan(0) */
    seq_t catCurrent = (seq_t)1;   /* C(0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute B(k-1) = sum_{i=0}^{k-1} a(i) * AA[k-1-i] */
        seq_t B = (seq_t)0;
        for (idx_t i = 0; i < k; ++i) {
            B += a_arr[i] * AA[(k - 1) - i];
        }

        /* a(k) = Catalan(k-1) + B(k-1) */
        a_arr[k] = catCurrent + B;

        /* Update AA[k] = sum_{i=0}^{k} a(i) * a(k-i) */
        seq_t newAA = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i) {
            newAA += a_arr[i] * a_arr[k - i];
        }
        AA[k] = newAA;

        /* Prepare Catalan(k) for next iteration:
           C(k) = C(k-1) * (4k-2) / (k+1)  (integer division) */
        if (k < n) {   /* no need to compute after final a(n) */
            seq_t num = (seq_t)(4 * k - 2);
            seq_t den = (seq_t)(k + 1);
            catCurrent = catCurrent * num / den;
        }
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(AA);
    return result;
}
