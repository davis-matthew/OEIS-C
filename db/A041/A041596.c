/*
 * A041596  Numerators of continued fraction convergents to sqrt(316).
 * https://oeis.org/A041596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041596 – Numerators of continued fraction convergents to sqrt(316)
   Recurrence: a(0)=17, for n≥1 a(n)=b(n)·a(n-1)+a(n-2),
   where b(n) are the partial quotients after the initial term:
   1,3,2,8,2,3,1,34 repeating. */
#include <stddef.h>

static const unsigned int period_int[8] = {
    1, 3, 2, 8,
    2, 3, 1, 34
};

seq_t a(idx_t n)
{
    seq_t p_minus2 = (seq_t)0; /* p_{-2} */
    seq_t p_minus1 = (seq_t)1; /* p_{-1} */
    seq_t p_cur    = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai;
        if (i == 0)
            ai = (seq_t)17;
        else
            ai = (seq_t)period_int[(i - 1) & 7]; /* (i-1) % 8 */

        /* p_i = ai * p_{i-1} + p_{i-2} */
        seq_t prod = ai * p_minus1;
        p_cur = prod + p_minus2;

        /* shift for next iteration */
        p_minus2 = p_minus1;
        p_minus1 = p_cur;
    }
    return p_cur;
}
