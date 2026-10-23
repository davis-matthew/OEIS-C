/*
 * A011653  115th cyclotomic polynomial.
 * https://oeis.org/A011653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in Φ_115(x) (the 115th cyclotomic
 *            polynomial)
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t coeff[89];
static bool ready = false;

static void init_coeff(void) {
    /* Numerator: Φ_5(x^23) = 1 + x^23 + x^46 + x^69 + x^92  */
    seq_t N[93] = {0};          /* indices 0..92 */
    N[0]  = 1;
    N[23] = 1;
    N[46] = 1;
    N[69] = 1;
    N[92] = 1;

    /* Denominator: Φ_5(x) = 1 + x + x^2 + x^3 + x^4 */
    for (int i = 92; i >= 4; --i) {
        seq_t q = N[i];
        coeff[i - 4] = q;
        for (int j = 0; j < 5; ++j)
            N[i - j] -= q;
    }
    ready = true;
}

seq_t a(idx_t n) {
    if (!ready) init_coeff();
    if (n < 89) return coeff[n];
    return (seq_t)0;
}
