/*
 * A016194  Inverse of 2185th cyclotomic polynomial.
 * https://oeis.org/A016194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) are the coefficients of the formal power series expansion *
 *            of 1/Φ_2185(x) (the inverse of the 2185th cyclotomic
 *            polynomial). The * sequence is periodic with period length 2185
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define PERIOD_LEN 2185

static seq_t period[PERIOD_LEN];
static int initialized = 0;

static void init_period(void)
{
    seq_t *cur = (seq_t *)calloc(PERIOD_LEN, sizeof(seq_t));
    cur[0] = 1;

    /* Positive exponent factors: (1 - x^d) with d = 1, 95, 115, 437.
       Apply in-place subtraction from high to low indices. */
    /* d = 437 */
    for (size_t k = PERIOD_LEN - 1; k >= 437; --k)
        cur[k] -= cur[k - 437];
    /* d = 115 */
    for (size_t k = PERIOD_LEN - 1; k >= 115; --k)
        cur[k] -= cur[k - 115];
    /* d = 95  */
    for (size_t k = PERIOD_LEN - 1; k >= 95; --k)
        cur[k] -= cur[k - 95];
    /* d = 1   */
    for (size_t k = PERIOD_LEN - 1; k >= 1; --k)
        cur[k] -= cur[k - 1];

    /* Negative exponent factors: 1/(1 - x^d) with d = 5, 19, 23.
       Apply in-place accumulation from low to high indices. */
    /* d = 5  */
    for (size_t k = 0; k < PERIOD_LEN; ++k)
        if (k >= 5) cur[k] += cur[k - 5];
    /* d = 19 */
    for (size_t k = 0; k < PERIOD_LEN; ++k)
        if (k >= 19) cur[k] += cur[k - 19];
    /* d = 23 */
    for (size_t k = 0; k < PERIOD_LEN; ++k)
        if (k >= 23) cur[k] += cur[k - 23];

    /* Copy result into the static period array. */
    for (size_t i = 0; i < PERIOD_LEN; ++i)
        period[i] = cur[i];

    free(cur);
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_period();

    size_t idx = (size_t)(n % PERIOD_LEN);
    return period[idx];
}
