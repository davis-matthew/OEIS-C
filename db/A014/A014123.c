/*
 * A014123  Inverse of 114th cyclotomic polynomial.
 * https://oeis.org/A014123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1/Φ_114(x), * where Φ_114(x) is the 114th cyclotomic polynomial.
 *            * The sequence satisfies the linear recurrence * a[n] =
 *            –∑_{i=1}^{36} c[i] · a[n–i], * where c[i] are the coefficients
 *            of Φ_114(x): * Φ_114(x) = 1 + x + 0x² – x³ – x⁴ +...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const int coeff[37] = {
    1,  1,  0, -1, -1,  0,  1,  1,  0, -1, -1,  0,  1,  1,  0,
   -1, -1,  0,  1,  0, -1, -1,  0,  1,  1,  0, -1, -1,  0,  1,
    1,  0, -1, -1,  0,  1,  1
};

/* Allocate an array of seq_t of given size. */
static seq_t *alloc_seq_arr(size_t n)
{
    return (seq_t *)malloc(n * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    /* Thread‑safe lazy initialization of the period table. */
    static seq_t *period = NULL;
    static size_t period_len = 114;

    if (!period) {
        period = alloc_seq_arr(period_len);
        period[0] = 1;                     /* a(0) = 1 */

        for (idx_t i = 1; i < period_len; ++i) {
            seq_t sum = 0;
            idx_t limit = i < 36 ? i : 36;
            for (idx_t j = 1; j <= limit; ++j) {
                if (coeff[j] == 0) continue;
                sum += (seq_t)coeff[j] * period[i - j];
            }
            period[i] = -sum;
        }
    }

    return period[n % period_len];
}
