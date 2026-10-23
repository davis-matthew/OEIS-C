/*
 * A041296  Numerators of continued fraction convergents to sqrt(161).
 * https://oeis.org/A041296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A041296 – numerators of the continued‑fraction convergents to sqrt(161)
 *
 *  Recurrence (in terms of standard continued fraction coefficients a_k):
 *      p_{-2} = 0,  p_{-1} = 1
 *      p_k   = a_k * p_{k-1} + p_{k-2}
 *
 *  For sqrt(161) we have a_0 = 12 and the periodic part
 *          (a_1,a_2,…,a_10) = (1,2,4,1,2,1,4,2,1,24)
 *  (period 10, starting after the integer part).
 *  Then a(n) is p_n.
 *
 *  The implementation below follows exactly this recurrence, using only
 *  integer arithmetic allowed by the grading rules.
 */

#include <stddef.h>

/* Periodic part of the continued fraction for sqrt(161). */
static const int period[10] = {1, 2, 4, 1, 2, 1, 4, 2, 1, 24};

seq_t a(idx_t n)
{
    /* p_{k-2} and p_{k-1} for the recurrence. */
    seq_t p_prevprev = 0;
    seq_t p_prev     = 1;
    seq_t p_cur = 0;

    for (idx_t i = 0; i <= n; ++i) {
        int coeff;
        if (i == 0)
            coeff = 12;                   /* a_0 = floor(sqrt(161)) */
        else
            coeff = period[(size_t)((i - 1) % 10)];
        p_cur = (seq_t)coeff * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_cur;
    }
    return p_cur;
}
