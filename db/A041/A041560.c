/*
 * A041560  Numerators of continued fraction convergents to sqrt(298).
 * https://oeis.org/A041560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(298).
   Recurrence:
     p[-2] = 0, p[-1] = 1
     p[n]   = a[n] * p[n-1] + p[n-2]   for n >= 0
   where a[0] = 17 and the partial quotients a[1], a[2], ... are
   periodic with period 3,1,4,5,1,1,5,4,1,3,34. */

seq_t a(idx_t n)
{
    /* contiguous part of the continued fraction after the integer part */
    static const seq_t period[] = {3, 1, 4, 5, 1, 1, 5, 4, 1, 3, 34};
    const unsigned period_len = sizeof(period) / sizeof(period[0]);

    seq_t p_prev2 = 0;   /* p[-2] */
    seq_t p_prev1 = 1;   /* p[-1] */
    seq_t p_curr  = 0;   /* p[n] */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff;
        if (i == 0)
            coeff = 17;                       /* a[0] */
        else
            coeff = period[(unsigned)((i - 1) % period_len)]; /* a[i] */

        p_curr = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_curr;
}
