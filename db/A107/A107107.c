/*
 * A107107  For each partition of n, calculate (dM2/dM3) where dM2 =
 *          A036039(p) and dM3 = A036040(p); then sum over all partitions of
 *          n.
 * https://oeis.org/A107107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of x^n in 1/∏_{m≥1} (1-(m-1)!·x^m) = sum over
 *            partitions of n of product_i (c_i-1)!^k_i
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* f[k] will hold a(k) */
    seq_t *f = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!f) return 0;                /* allocation failed */
    f[0] = 1;
    for (idx_t i = 1; i <= n; ++i) f[i] = 0;

    seq_t fact = 1;                  /* factorial of (m-1)   */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t base = fact;           /* (m-1)!              */
        for (idx_t k = m; k <= n; ++k) {
            f[k] += base * f[k - m];
        }
        fact *= (seq_t)m;            /* prepare (m)!         */
    }

    seq_t result = f[n];
    free(f);
    return result;
}
