/*
 * A016428  511th cyclotomic polynomial.
 * https://oeis.org/A016428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient n of the 511th cyclotomic polynomial Φ511(x)
 * Range:     i8 n <= 432, i16 n <= 432, i32 n <= 432, i64 n <= 432,
 *            i128 n <= 432, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* static table of the 433 coefficients of Φ₅₁₁(x); 0 if n>432 */
    static seq_t coeff[433];
    static int init = 0;   /* 0 → not yet computed, 1 → ready */

    if (!init) {
        /* -------- Numerator N(x) = (x^511-1)(x-1) = x^512 - x^511 - x + 1 */
        static seq_t N[513];   /* indices 0..512 */
        for (idx_t i = 0; i <= 512; ++i) N[i] = 0;
        N[512] = 1;   /* x^512   */
        N[511] = -1;  /* -x^511  */
        N[1]   = -1;  /* -x      */
        N[0]   = 1;   /* +1      */

        /* -------- Denominator D(x) = (x^73-1)(x^7-1)
           = x^80 - x^73 - x^7 + 1 */
        static seq_t D[81];   /* indices 0..80 */
        for (idx_t i = 0; i <= 80; ++i) D[i] = 0;
        D[80] = 1;   /* x^80   */
        D[73] = -1;  /* -x^73  */
        D[7]  = -1;  /* -x^7   */
        D[0]  = 1;   /* +1     */

        /* -------- Division N(x) / D(x) gives Φ₅₁₁(x) */
        static seq_t Q[433];   /* indices 0..432 */
        for (idx_t i = 0; i <= 432; ++i) Q[i] = 0;

        /* Long division */
        for (idx_t j = 512; j >= 80; --j) {
            seq_t lead = N[j];
            if (lead != 0) {
                Q[j - 80] = lead;          /* quotient coefficient */
                idx_t shift = j - 80;
                for (idx_t k = 0; k <= 80; ++k) {
                    if (D[k] != 0) {
                        N[shift + k] -= lead * D[k];
                    }
                }
            }
        }

        /* Copy the computed coefficients for future calls */
        for (idx_t i = 0; i <= 432; ++i) coeff[i] = Q[i];
        init = 1;
    }

    return (n <= 432) ? coeff[(size_t)n] : (seq_t)0;
}
