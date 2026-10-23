/*
 * A015187  Inverse of 1178th cyclotomic polynomial.
 * https://oeis.org/A015187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the nth coefficient of the power series * 1/Φ_1178(x),
 *            the inverse of the 1178th cyclotomic polynomial. * It is
 *            periodic with period 1178. * The series can be written as: * (x
 *            – 1) * (x^2 – 1)^(-1) * * (x^19 – 1)^(-1) * (x^31 – 1)^(-1) * *
 *            (x^38 – 1) * (x^62 – 1) * (x^589 – 1) * (the fact...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdbool.h>

#define PERIOD 1178

static seq_t period[PERIOD];
static bool period_ready = false;

static void init_period(void)
{
    if (period_ready) return;

    static seq_t buf1[PERIOD];
    static seq_t buf2[PERIOD];
    seq_t *cur  = buf1;
    seq_t *next = buf2;

    /* start with the series 1 */
    for (int i = 0; i < PERIOD; ++i) cur[i] = 0;
    cur[0] = 1;

    /* Factor (x – 1)   :  coeff[0] = -1, coeff[1] = +1 */
    for (int i = 0; i < PERIOD; ++i) {
        next[i] = -cur[i];
        if (i >= 1) next[i] += cur[i - 1];
    }
    /* swap buffers */
    seq_t *tmp = cur; cur = next; next = tmp;

    /* Factor (x^2 – 1)^(-1)  :  coeff[n] = 1 if n≡0 (mod 2) */
    for (int i = 0; i < PERIOD; ++i) {
        next[i] = cur[i];
        if (i >= 2) next[i] += next[i - 2];
    }
    tmp = cur; cur = next; next = tmp;

    /* Factor (x^19 – 1)^(-1) :  coeff[n] = 1 if n≡0 (mod 19) */
    for (int i = 0; i < PERIOD; ++i) {
        next[i] = cur[i];
        if (i >= 19) next[i] += next[i - 19];
    }
    tmp = cur; cur = next; next = tmp;

    /* Factor (x^31 – 1)^(-1) :  coeff[n] = 1 if n≡0 (mod 31) */
    for (int i = 0; i < PERIOD; ++i) {
        next[i] = cur[i];
        if (i >= 31) next[i] += next[i - 31];
    }
    tmp = cur; cur = next; next = tmp;

    /* Factor (x^38 – 1)   :  coeff[0] = -1, coeff[38] = +1 */
    for (int i = 0; i < PERIOD; ++i) {
        next[i] = -cur[i];
        if (i >= 38) next[i] += cur[i - 38];
    }
    tmp = cur; cur = next; next = tmp;

    /* Factor (x^62 – 1)   :  coeff[0] = -1, coeff[62] = +1 */
    for (int i = 0; i < PERIOD; ++i) {
        next[i] = -cur[i];
        if (i >= 62) next[i] += cur[i - 62];
    }
    tmp = cur; cur = next; next = tmp;

    /* Factor (x^589 – 1)  :  coeff[0] = -1, coeff[589] = +1 */
    for (int i = 0; i < PERIOD; ++i) {
        next[i] = -cur[i];
        if (i >= 589) next[i] += cur[i - 589];
    }
    tmp = cur; cur = next; next = tmp;

    /* copy the computed period into the global array */
    memcpy(period, cur, PERIOD * sizeof(seq_t));
    period_ready = true;
}

seq_t a(idx_t n)
{
    init_period();
    return period[(size_t)(n % PERIOD)];
}
