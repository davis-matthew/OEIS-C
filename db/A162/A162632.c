/*
 * A162632  G.f. is the polynomial (Product_{k=1..14} (1 - x^(3*k)))/(1-x)^14.
 * https://oeis.org/A162632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..14} (1 + x + … + x^{3k-1})
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 301, u128 n <= 301,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the polynomial  (∏_{k=1}^{14}(1-x^{3k}))/ (1-x)^{14}  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The polynomial has degree 301 (sum_{k=1}^{14}(3k-1)). */
    enum { MAX_DEG = 301, SIZE = MAX_DEG + 1 };
    static seq_t coeff[SIZE];
    static seq_t tmp[SIZE];
    static idx_t maxdeg = 0;
    static int initialized = 0;

    if (!initialized) {
        /* First call: build the polynomial. */
        for (idx_t i = 0; i < SIZE; ++i)
            coeff[i] = 0;
        coeff[0] = 1;
        maxdeg = 0;

        for (idx_t k = 1; k <= 14; ++k) {
            idx_t len = 3 * k;              /* number of terms in current factor */
            idx_t new_max = maxdeg + len - 1;

            /* zero temporary array up to new_max */
            for (idx_t i = 0; i <= new_max; ++i)
                tmp[i] = 0;

            /* convolution: tmp[j] = sum coeff[i] for i + shift = j */
            for (idx_t i = 0; i <= maxdeg; ++i) {
                seq_t val = coeff[i];
                if (!val) continue; /* skip zero terms */
                for (idx_t s = 0; s < len; ++s) {
                    tmp[i + s] += val;
                }
            }

            /* copy back to coeff and update maxdeg */
            for (idx_t i = 0; i <= new_max; ++i)
                coeff[i] = tmp[i];
            maxdeg = new_max;
        }
        initialized = 1;
    }

    return (n <= maxdeg) ? coeff[n] : (seq_t)0;
}
