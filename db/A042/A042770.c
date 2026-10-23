/*
 * A042770  Numerators of continued fraction convergents to sqrt(916).
 * https://oeis.org/A042770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 16, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(916)
   Uses recurrence:  p_n = a_n p_{n-1} + p_{n-2}
   where the continued‑fraction coefficients a_n for sqrt(916) are
   periodic after a0. */
#include <stdint.h>

/* Static storage for the periodic part of the continued fraction
   of sqrt(916).  The period length is at most 512 (well beyond
   the known value). */
static idx_t period[512];
static idx_t period_len = 0;

/* Compute the periodic part on first call */
static void init_period(void)
{
    /* D = 916  */
    const idx_t D = 916;
    /* a0 = floor(sqrt(D)) */
    idx_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) ++a0;

    /* Continued‑fraction generator */
    idx_t m = 0, d = 1, a = a0;
    period_len = 0;
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[period_len++] = a;
    } while (a != 2 * a0);   /* termination condition for sqrt(D) */
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    /* Initialize period if first call */
    if (!period_len) init_period();

    /* Handle the recurrence */
    seq_t p_prev2 = 0;      /* p_{-2} = 0   */
    seq_t p_prev1 = 1;      /* p_{-1} = 1   */
    seq_t p_curr;

    /* a0 is the integer part */
    const idx_t D = 916;
    idx_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) ++a0;

    for (idx_t i = 0; i <= n; ++i) {
        idx_t coeff;
        if (i == 0) {
            coeff = a0;                 /* a_0 = floor(sqrt(916)) */
        } else {
            coeff = period[(i - 1) % period_len]; /* periodic part */
        }
        /* Convert coeff to seq_t for multiplication */
        p_curr = (seq_t)coeff * p_prev1 + p_prev2;
        if (i == n) return p_curr;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    /* Should never reach here */
    return (seq_t)0;
}
