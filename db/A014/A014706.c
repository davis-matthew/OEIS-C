/*
 * A014706  Inverse of 697th cyclotomic polynomial.
 * https://oeis.org/A014706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ_697(x), where Φ_697(x) is the
 *            697‑th cyclotomic polynomial. The sequence is periodic with
 *            period 697
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t coeff[641] = {0};    /* coefficients of Φ_697(x), 0..640 */
static seq_t period[697] = {0};   /* first 697 terms of 1/Φ_697(x) */
static bool ready = false;

static void init(void)
{
    seq_t A[699];
    seq_t B[59];
    size_t i, j;

    /* zero initialise the arrays */
    for (i = 0; i < 699; ++i) A[i] = 0;
    for (i = 0; i < 59;  ++i) B[i] = 0;

    /* A(x) = (x^697 - 1)(x - 1) = x^698 - x^697 - x + 1 */
    A[698] = 1;
    A[697] = -1;
    A[1]   = -1;
    A[0]   = 1;

    /* B(x) = (x^41 - 1)(x^17 - 1) = x^58 - x^41 - x^17 + 1 */
    B[58] = 1;
    B[41] = -1;
    B[17] = -1;
    B[0]  = 1;

    /* Long division: A(x) / B(x)  →  Φ_697(x)  (degree 640) */
    for (i = 0; i < 641; ++i) coeff[i] = 0;
    for (long long deg = 698; deg >= 58; --deg) {
        long long shift = deg - 58;
        seq_t c = A[deg];
        coeff[shift] = c;
        if (c != 0) {
            for (j = 0; j <= 58; ++j) {
                A[shift + j] -= c * B[j];
            }
        }
    }

    /* Compute the first 697 terms of 1/Φ_697(x) by the recurrence */
    period[0] = 1;
    for (i = 1; i < 697; ++i) {
        seq_t sum = 0;
        size_t max_i = (i <= 640 ? i : 640);
        for (j = 1; j <= max_i; ++j) {
            sum += coeff[j] * period[i - j];
        }
        period[i] = -sum;
    }

    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) init();
    return period[n % 697];
}
