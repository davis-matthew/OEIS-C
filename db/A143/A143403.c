/*
 * A143403  Expansion of x^k/Product_{t=k..2k} (1-tx) for k=8.
 * https://oeis.org/A143403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in x^8/∏_{t=8}^{16} (1 - t x)
 *            Equivalently, for n ≥ 8: a(n) = Σ_{k_8+…+k_16 = n-8}
 *            ∏_{t=8}^{16} t^{k_t} The DP below computes these coefficients
 *            efficiently
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 13, u64 n <= 20, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for calloc, free */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* n < 8 → 0 */
    if (n < 8) {
        seq_t zero = 0;
        return zero;
    }

    /* m = n - 8 */
    idx_t m = n - 8;

    /* allocate f[0..m] */
    seq_t *f = (seq_t *)calloc((size_t)m + 1U, sizeof(seq_t));
    if (!f) {
        /* allocation failure; return 0 as fallback */
        seq_t zero = 0;
        return zero;
    }

    f[0] = 1;  /* base of convolution */

    /* iterate over t = 8 .. 16 */
    for (idx_t t = 8; t <= 16; ++t) {
        /* in-place update: f[s] = f_prev[s] + t * f[s-1] */
        for (idx_t s = 1; s <= m; ++s) {
            f[s] = f[s] + (t * f[s - 1]);
        }
    }

    seq_t result = f[m];
    free(f);
    return result;
}
