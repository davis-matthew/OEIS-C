/*
 * A184358  a(n) = (n+1)!^2/2^n.
 * https://oeis.org/A184358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)!^2) / 2^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = (n+1)^2 * a(n-1) / 2  for n > 0 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* start with a(0) = 1 */
    seq_t res = (seq_t)1;

    /* iteratively apply the recurrence up to n */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* m = (i+1)^2, cast to seq_t before every operation */
        seq_t m = (seq_t)(i + (idx_t)1) * (seq_t)(i + (idx_t)1);

        /* to avoid overflow we divide by 2 before the final multiplication.
           Exactly one of 'res' or 'm' is even. */
        seq_t a_val = res;
        seq_t b_val = m;

        if ((a_val & (seq_t)1) == 0)  /* a_val even */
            a_val = a_val / (seq_t)2;
        else
            b_val = b_val / (seq_t)2;

        res = a_val * b_val;
    }

    return res;
}
