/*
 * A041408  Numerators of continued fraction convergents to sqrt(219).
 * https://oeis.org/A041408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to √219, obtained from
 *            the continued fraction expansion sqrt(219) = [14; 1, 3, 1, 28,
 *            1, 3, 1, 28, … ]. The periodic part has length 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External types provided by the build system:
   typedef ... seq_t;
   typedef ... idx_t;  (typically uint64_t)  */

seq_t a(idx_t n)
{
    /* Handle n = 0 immediately. */
    if (n == 0)
        return (seq_t)14;

    seq_t prev2 = 0;   /* p_{-2}  */
    seq_t prev1 = 1;   /* p_{-1}  */
    seq_t cur;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff;   /* a_i */

        if (i == 0)
            coeff = (seq_t)14;
        else
        {
            /* Period [1,3,1,28] repeats after the first coefficient. */
            idx_t k = (i - 1) % 4;
            if (k == 0)
                coeff = (seq_t)1;
            else if (k == 1)
                coeff = (seq_t)3;
            else if (k == 2)
                coeff = (seq_t)1;
            else            /* k == 3 */
                coeff = (seq_t)28;
        }

        /* Recurrence: p_i = coeff * p_{i-1} + p_{i-2} */
        cur = coeff * prev1 + prev2;

        if (i == n)
            return cur;

        prev2 = prev1;
        prev1 = cur;
    }

    /* Should never reach here.  */
    return 0;
}
