/*
 * A027404  Number of labeled servers of dimension 17.
 * https://oeis.org/A027404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(n,17)} C(17,k) · a(n‑k) · ∏_{m=1}^{k‑1} (n –
 *            m), with a(0) = 1. The product is empty for k = 1 and equals 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Binomial coefficients C(17,k) for k=0..17 */
    static const seq_t C[18] = {
        1, 17, 136, 680, 2380, 6188, 12376, 19448, 24310,
        24310, 19448, 12376, 6188, 2380, 680, 136, 17, 1
    };

    /* Allocate array for a[0..n] */
    seq_t *a = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!a)
        return (seq_t)0; /* Out of memory: return 0 as a safe fallback */

    a[0] = (seq_t)1;

    for (idx_t cur = 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        idx_t kmax = cur < 17 ? cur : 17;
        for (idx_t k = 1; k <= kmax; ++k) {
            /* Compute product_{m=1}^{k-1} (cur - m) */
            seq_t prod = (seq_t)1;
            for (idx_t m = 1; m < k; ++m)
                prod *= (seq_t)(cur - m);

            /* Add term: C(17,k) * a[cur-k] * prod */
            sum += C[k] * a[cur - k] * prod;
        }
        a[cur] = sum;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
