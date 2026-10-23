/*
 * A035025  Constant term of (normalized Delta)^(-n).
 * https://oeis.org/A035025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) is the coefficient of q^n in (∏_{m≥1}(1‑q^m)^{24})^{-n}
 *            Recurrence: a(0) = 1 a(m) = (24*n/m) * Σ_{k=1}^{m} σ₁(k) *
 *            a(m‑k) for m ≥ 1 where σ₁(k) is the sum of divisors of k. This
 *            formula follows from the logarithmic expansion of the product.
 *            The implementation uses dynamic programming and run...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate and compute σ₁(k) for k = 1 … n */
    idx_t  *sigma = (idx_t*)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!sigma) return 0;          /* allocation failure: return 0 */
    for (idx_t i = 1; i <= n; ++i) sigma[i] = 0;
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += i;

    /* Allocate array for a[0 … n] */
    seq_t *a = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a) { free(sigma); return 0; }
    a[0] = 1;                     /* a(0) = 1 */

    for (idx_t m = 1; m <= n; ++m) {
        unsigned __int128 sum = 0; /* Σ σ₁(k) * a(m‑k) */
        for (idx_t k = 1; k <= m; ++k) {
            sum += (unsigned __int128)sigma[k] * (unsigned __int128)a[m - k];
        }
        unsigned __int128 temp = sum * (unsigned __int128)n * 24;
        temp /= m;
        a[m] = (seq_t)temp;
    }

    seq_t result = a[n];

    free(a);
    free(sigma);
    return result;
}
