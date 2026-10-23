/*
 * A042638  Numerators of continued fraction convergents to sqrt(849).
 * https://oeis.org/A042638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(849)
   Recurrence:   p[-2] = 0,  p[-1] = 1
                 p[n]  = a[n] * p[n-1] + p[n-2]
   where a[0] = 29 and for n >= 1 the partial quotient a[n] repeats:
      7, 3, 1, 2, 1, 7, 1, 1, 2, 4, 11,
      2, 2, 1, 18, 1, 2, 2, 11, 4, 2,
      1, 1, 7, 1, 2, 1, 3, 7, 58  (period length 30)
*/

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(849) */
    static const int period[30] = {
        7, 3, 1, 2, 1, 7, 1, 1, 2, 4, 11,
        2, 2, 1, 18, 1, 2, 2, 11, 4, 2,
        1, 1, 7, 1, 2, 1, 3, 7, 58
    };

    seq_t p_prev2 = 0;   /* p[-2] */
    seq_t p_prev1 = 1;   /* p[-1] */
    seq_t p = 0;         /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        int a_i;
        if (i == 0)
            a_i = 29;                    /* a[0] */
        else
            a_i = period[(i - 1) % 30];   /* a[n] for n >= 1 */

        p = (seq_t)a_i * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p;
}
