/*
 * A169574  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 34: a(n) = 2 + 31 * Σ_{i=0}^{n-1} a(i)
 *            a(35) = 31 * Σ_{i=1}^{34} a(i) – 495 For n ≥ 36: a(n) = 31 *
 *            Σ_{i=n-34}^{n-1} a(i) – 496 * a(n-35) (Recurrence derived from
 *            the generating function of A169574.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Base case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate space for a[0..n] */
    seq_t *v = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!v)
        return (seq_t)1;            /* fallback – unlikely to happen */

    /* a[0] = 1 */
    v[0] = (seq_t)1;
    seq_t prefix_sum = v[0];          /* Σ_{i=0}^{k} a(i) for current k */

    /* Compute a[1] … a[34] */
    if (n >= 1) {
        for (idx_t i = 1; i <= n && i <= 34; ++i) {
            v[i] = (seq_t)2 + mul_small(prefix_sum, 31);
            prefix_sum += v[i];
        }
        if (n <= 34) {
            seq_t res = v[n];
            free(v);
            return res;
        }
    }

    /* a[35] = 31 * Σ_{i=1}^{34} a(i) – 495 */
    seq_t sum1_34 = prefix_sum - v[0];          /* Σ_{i=1}^{34} a(i) */
    v[35] = mul_small(sum1_34, 31) - (seq_t)495;
    if (n == 35) {
        seq_t res = v[35];
        free(v);
        return res;
    }

    /* sliding sum of the last 34 terms: for i = 36 … n */
    seq_t running_sum = 0;
    for (idx_t j = 2; j <= 35; ++j)
        running_sum += v[j];                   /* sum v[2]…v[35] */

    for (idx_t i = 36; i <= n; ++i) {
        v[i] = mul_small(running_sum, 31)
               - mul_small(v[i - 35], 496);
        running_sum += v[i];
        running_sum -= v[i - 35];
    }

    seq_t result = v[n];
    free(v);
    return result;
}
