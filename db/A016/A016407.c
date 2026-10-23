/*
 * A016407  427th cyclotomic polynomial.
 * https://oeis.org/A016407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 427th cyclotomic polynomial
 *            Φ_427(x)
 * Range:     i8 n <= 360, i16 n <= 360, i32 n <= 360, i64 n <= 360,
 *            i128 n <= 360, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t coeff[361];
    static bool ready = false;

    if (!ready) {
        seq_t narr[429] = {0};
        /* numerator: (x-1)(x^427-1) = x^428 - x^427 - x + 1 */
        narr[428] = (seq_t)1;
        narr[427] = (seq_t)-1;
        narr[1]   = (seq_t)-1;
        narr[0]   = (seq_t)1;

        seq_t darr[69] = {0};
        /* denominator: (x^7-1)(x^61-1) = x^68 - x^61 - x^7 + 1 */
        darr[0]   = (seq_t)1;
        darr[7]   = (seq_t)-1;
        darr[61]  = (seq_t)-1;
        darr[68]  = (seq_t)1;

        const idx_t N = 428;   /* degree of numerator   */
        const idx_t D = 68;    /* degree of denominator */
        for (idx_t k = N; k >= D; --k) {
            seq_t c = narr[k];
            coeff[k - D] = c;
            if (c != (seq_t)0) {
                /* subtract c * darr shifted by (k-D) from narr */
                for (idx_t i = 0; i <= D; ++i) {
                    narr[(k - D) + i] -= c * darr[i];
                }
            }
        }
        ready = true;
    }

    if (n >= 361) return (seq_t)0;
    return coeff[n];
}
