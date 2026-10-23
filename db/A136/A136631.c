/*
 * A136631  Antidiagonal sums of triangle A136630, omitting antidiagonals of
 *          all zeros.
 * https://oeis.org/A136631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} coefficient of y^{n-k} in ∏_{j=0..⌊k/2⌋}
 *            1/(1 - (2j + k - 2⌊k/2⌋)^2 y) where y = x². This implementation
 *            computes the sums by building the weighted partition functions
 *            for even and odd squares incrementally
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*-------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Arrays that hold the weighted partition coefficients
       for the current set of even and odd squares. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp_even = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *dp_odd  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp_even || !dp_odd) { /* allocation failure */
        return (seq_t)0;
    }

    /* Initialise to zero and set the empty‑partition value to 1. */
    for (size_t i = 0; i < sz; ++i) {
        dp_even[i] = 0;
        dp_odd[i]  = 0;
    }
    dp_even[0] = dp_odd[0] = (seq_t)1;

    seq_t result = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* The new square that appears when k is added. */
            seq_t s = (seq_t)(k * k);      /* k*k fits in 64‑bit */
            if ((k & 1) == 0) {            /* even k  → even‑square set */
                for (idx_t p = 1; p <= n - k; ++p) {
                    dp_even[p] += s * dp_even[p - 1];
                }
            } else {                       /* odd k   → odd‑square set  */
                for (idx_t p = 1; p <= n - k; ++p) {
                    dp_odd[p] += s * dp_odd[p - 1];
                }
            }
        }

        /* Add the current contribution to the answer. */
        if ((k & 1) == 0)
            result += dp_even[n - k];
        else
            result += dp_odd[n - k];
    }

    free(dp_even);
    free(dp_odd);
    return result;
}
