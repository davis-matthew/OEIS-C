/*
 * A016355  203rd cyclotomic polynomial.
 * https://oeis.org/A016355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 168, i16 n <= 168, i32 n <= 168, i64 n <= 168,
 *            i128 n <= 168, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = coefficient of x^n in the 203rd cyclotomic polynomial
   Φ203(x) = (x^203 – 1) / ((x – 1)·Φ7(x)·Φ29(x)) */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    const idx_t DEG = 168;          /* φ(203) = 168 */
    if (n > DEG) return (seq_t)0;

    static seq_t coeff[169];
    static bool ready = false;
    if (!ready) {
        const idx_t N = 203;
        const idx_t DEG_DIV = 35;   /* deg((x-1)·Φ7·Φ29) = 35 */

        /* numerator polynomial: x^203 – 1 */
        static seq_t num[204] = {0};
        num[N] = (seq_t)1;
        num[0] = (seq_t)-1;

        /* component polynomials */
        static const seq_t poly1[2] = {(seq_t)-1, (seq_t)1};            /* x – 1 */
        static const seq_t poly7[7] = {
            (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
            (seq_t)1, (seq_t)1, (seq_t)1                      /* Φ7 = x⁶+…+1 */
        };
        static const seq_t poly29[29] = {
            (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
            (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
            (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1
        }; /* Φ29 = x²⁸+…+1 */

        /* temp1 = poly1 * poly7  (degree 0..7) */
        static seq_t temp1[8] = {0};
        for (idx_t i = 0; i < 2; ++i)
            for (idx_t j = 0; j < 7; ++j)
                temp1[i + j] += poly1[i] * poly7[j];

        /* div = temp1 * poly29  (degree 0..35) */
        static seq_t div[36] = {0};
        for (idx_t i = 0; i < 8; ++i)
            for (idx_t j = 0; j < 29; ++j)
                div[i + j] += temp1[i] * poly29[j];

        /* Perform long division num / div, recording the quotient in coeff */
        for (idx_t d = N; ; --d) {
            if (num[d] != 0) {
                seq_t c = num[d];
                idx_t shift = d - DEG_DIV;    /* position in quotient */
                coeff[shift] = c;
                for (idx_t j = 0; j <= DEG_DIV; ++j)
                    num[shift + j] -= c * div[j];
            }
            if (d == DEG_DIV) break;
        }

        ready = true;
    }
    return coeff[(idx_t)n];
}
