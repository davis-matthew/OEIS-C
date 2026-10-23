/*
 * A015757  Inverse of 1748th cyclotomic polynomial.
 * https://oeis.org/A015757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the 1748th cyclotomic polynomial.
   The coefficients form a periodic sequence with period 1748.
   They are generated as the product
   ∏_{d|1748} (1 - x^d)^{-μ(1748/d)},
   where μ is the Möbius function. */

/* Note: the sequence is period‑1748; coefficients for n ≥ 0 are
   retrieved modulo 1748. */

#include <stdbool.h>
#include <stddef.h>

static seq_t coeff[1748];
static bool ready = false;

seq_t a(idx_t n)
{
    if (!ready) {
        const idx_t N = 1748;
        /* initialise coefficients – only coeff[0] = 1 */
        for (idx_t i = 0; i < N; ++i) coeff[i] = 0;
        coeff[0] = 1;

        /* factors with exponent +1 :  (1 - x^k)   (k = 2, 76, 92, 874) */
        static const idx_t plus1[] = {2, 76, 92, 874};
        const int pcnt = sizeof(plus1) / sizeof(plus1[0]);

        for (int f = 0; f < pcnt; ++f) {
            idx_t k = plus1[f];
            /* multiply in place, loop from high to low to avoid overwrite */
            for (idx_t i = N - 1; i >= k; --i) {
                coeff[i] -= coeff[i - k];
            }
        }

        /* temporary array for inverse factors */
        static seq_t tmp[1748];

        /* factors with exponent -1 :  (1 - x^k)^{-1}
           (k = 4, 38, 46); the factor k = 1748 has no effect on indices
           < 1748 and is omitted. */
        static const idx_t minus1[] = {4, 38, 46};
        const int mcnt = sizeof(minus1) / sizeof(minus1[0]);

        for (int f = 0; f < mcnt; ++f) {
            idx_t k = minus1[f];
            tmp[0] = coeff[0];
            for (idx_t i = 1; i < N; ++i) {
                tmp[i] = coeff[i];
                if (i >= k) tmp[i] += tmp[i - k];  /* cumulative sum */
            }
            for (idx_t i = 0; i < N; ++i) coeff[i] = tmp[i];
        }

        ready = true;
    }

    return coeff[n % 1748];
}
