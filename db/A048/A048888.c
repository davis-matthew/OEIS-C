/*
 * A048888  a(n) = Sum_{m=1..n} T(m,n+1-m), array T as in A048887.
 * https://oeis.org/A048888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2}^{n+1} F_k(n) * where F_k is the k‑step
 *            Fibonacci sequence defined by * F_k(i)=0 for 0 ≤ i < k-1, *
 *            F_k(k-1)=1, * F_k(i)=∑_{t=1}^{k} F_k(i-t) for i ≥ k
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* a(0) = 0 */
    seq_t result = 0;
    /* Allocate workspace for a single k‑step Fibonacci sequence */
    seq_t *f = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!f) return 0;                  /* fallback on allocation failure */

    for (idx_t k = 2; k <= n + 1; ++k) {
        /* Initialise first k-1 terms (all zero) */
        for (idx_t i = 0; i < k - 1; ++i) f[i] = 0;

        /* kth term (index k‑1) */
        f[k - 1] = 1;

        /* Sliding window sum of the last k terms */
        seq_t sum = 1;                 /* sum of f[0 .. k-1] */

        /* Compute remaining terms up to index n */
        for (idx_t i = k; i <= n; ++i) {
            f[i] = sum;                /* current term */
            sum += f[i];               /* extend window */
            sum -= f[i - k];           /* drop the oldest term */
        }

        result += f[n];                /* add F_k(n) to answer */
    }

    free(f);
    return result;
}
