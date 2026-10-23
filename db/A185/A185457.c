/*
 * A185457  a(n) = abs( Im((2+i)^(2^n)) ).
 * https://oeis.org/A185457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = abs( Im((2+i)^(2^n)) )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute a(n) using repeated squaring of the complex number (2+i).
   The routine stores only absolute values of the real and imaginary
   parts and a sign bit for each part, thereby avoiding any signed
   intermediate values in the unsigned type seq_t. */
seq_t a(idx_t n)
{
    /* a = 2 + i*1 */
    seq_t abs_a = (seq_t)2;     /* |Re| */
    seq_t abs_b = (seq_t)1;     /* |Im| */
    bool  pos_a = true;         /* sign of Re > 0 */
    bool  pos_b = true;         /* sign of Im > 0 */

    for (idx_t i = 0; i < n; ++i)
    {
        /* Compute (a + ib)^2 = (a^2 - b^2) + 2ab i    */
        seq_t a2 = abs_a * abs_a;
        seq_t b2 = abs_b * abs_b;

        seq_t new_abs_a;
        bool   new_pos_a;
        /* real part: a^2 - b^2 */
        if (abs_a > abs_b)
        {
            new_abs_a = a2 - b2;
            new_pos_a = true;               /* positive */
        }
        else if (abs_a < abs_b)
        {
            new_abs_a = b2 - a2;
            new_pos_a = false;              /* negative */
        }
        else            /* equal */
        {
            new_abs_a = 0;
            new_pos_a = true;               /* sign is irrelevant */
        }

        /* imaginary part: 2ab */
        seq_t new_abs_b = 2 * abs_a * abs_b;
        bool   new_pos_b = (pos_a == pos_b);

        /* Update */
        abs_a = new_abs_a;
        pos_a = new_pos_a;
        abs_b = new_abs_b;
        pos_b = new_pos_b;
    }

    return abs_b;          /* |Im| */
}
