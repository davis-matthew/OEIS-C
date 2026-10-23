/*
 * A161930  Number of reduced words of length n in the Weyl group B_23.
 * https://oeis.org/A161930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 61, u128 n <= 529,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in ∏_{k=1..23} (1 - x^{2k})/(1 - x)
 *              i.e. the generating function of reduced words in Weyl group B_23.
 *
 * The generating function expands to a polynomial of degree 529 (23²).
 * This routine precomputes its coefficients once (in O(529²) time) and
 * then returns the requested term in O(1) time.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_DEG 529   /* maximum degree of the polynomial (23²) */

seq_t a(idx_t n)
{
    static seq_t coeff[MAX_DEG + 1];
    static bool initialized = false;

    if (!initialized) {
        /* coeff array is zero-initialised by the language. */
        coeff[0] = 1;
        idx_t cur_deg = 0;
        seq_t tmp[MAX_DEG + 1];

        for (idx_t k = 1; k <= 23; ++k) {
            idx_t factor_deg = 2 * k;        /* number of terms in (1 - x^{2k})/(1 - x) */

            /* zero temporary array */
            for (size_t i = 0; i <= (size_t)MAX_DEG; ++i) {
                tmp[i] = 0;
            }

            /* convolution with the current factor */
            for (idx_t i = 0; i <= cur_deg; ++i) {
                seq_t val = coeff[i];
                if (!val) continue;
                for (idx_t j = 0; j < factor_deg; ++j) {
                    tmp[i + j] += val;
                }
            }

            idx_t new_deg = cur_deg + (factor_deg - 1);
            for (idx_t i = 0; i <= new_deg; ++i) {
                coeff[i] = tmp[i];
            }
            cur_deg = new_deg;
        }

        initialized = true;
    }

    if (n > MAX_DEG) {
        return (seq_t)0;
    }
    return coeff[n];
}
