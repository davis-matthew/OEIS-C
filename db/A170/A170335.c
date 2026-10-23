/*
 * A170335  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36 * sum_{k=0}^{i-1} a(k) + 2 (for 1 ≤ n ≤ 41), a(0) = 1,
 *            a(42) = 36 * (sum_{k=0}^{41} a(k) - a(0)) - 666 * a(0) + 1, a(n)
 *            = 36 * sum_{k=1}^{41} a(n-k) - 666 * a(n-42) (for n ≥ 43)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Allocate array for 0..n */
    seq_t *a_vals = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a_vals) return (seq_t)0; /* allocation failure (unlikely) */

    a_vals[0] = (seq_t)1;
    seq_t total_sum = a_vals[0];                       /* Σ_{k=0}^{i-1} a(k) for current i   */

    for (idx_t i = 1; i <= n; i++) {
        if (i <= 41) {
            /* a(i) = 36 * (Σ_{k=0}^{i-1} a(k)) + 2 */
            a_vals[i] = (36 * total_sum) + (seq_t)2;
        } else if (i == 42) {
            /* a(42) = 36 * (Σ_{k=0}^{41} a(k) - a(0)) - 666 * a(0) + 1 */
            a_vals[i] = (36 * (total_sum - a_vals[0])) - (666 * a_vals[0]) + (seq_t)1;
        } else { /* i ≥ 43 */
            seq_t sum41 = 0;
            for (idx_t k = 1; k <= 41; k++) sum41 += a_vals[i - k];
            a_vals[i] = (36 * sum41) - (666 * a_vals[i - 42]);
        }
        total_sum += a_vals[i];   /* update Σ up to current i for next iteration  */
    }

    seq_t result = a_vals[n];
    free(a_vals);
    return result;
}
