/*
 * A016422  493rd cyclotomic polynomial.
 * https://oeis.org/A016422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 493rd cyclotomic polynomial.
 *            Since 493 = 17·29, we use Φ_{493}(x) = Φ_{17}(x^29)/Φ_{17}(x).
 *            Let D(x)=Φ_{17}(x)=1+x+⋯+x^16 and
 *            N(x)=Φ_{17}(x^29)=∑_{k=0}^{16}x^{29k}. Then the coefficient Q[i]
 *            of x^i in Q(x)=Φ_{493}(x) satisfies N[i] = ∑_{k=0}^{16} Q[i−k]
 *            (with Q[<0...
 * Range:     i8 n <= 448, i16 n <= 448, i32 n <= 448, i64 n <= 448,
 *            i128 n <= 448, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t coeff[449];          /* 0 … deg(Φ_493)=448 */

    if (!init) {
        /* pre‑compute all coefficients once */
        for (idx_t i = 0; i <= 448; ++i) {
            /* N[i] = 1 if i is a multiple of 29 and <= 464 (=16·29) */
            seq_t N = ((i % 29UL == 0) && (i / 29UL <= 16)) ? 1 : 0;

            seq_t sum = 0;
            for (idx_t j = 1; j <= 16 && i >= j; ++j)
                sum += coeff[i - j];

            coeff[i] = N - sum;        /* Q[i] */
        }
        init = true;
    }

    if (n <= 448)
        return coeff[n];
    else
        return 0;                      /* coefficients beyond degree are zero */
}
