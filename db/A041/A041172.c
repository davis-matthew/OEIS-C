/*
 * A041172  Numerators of continued fraction convergents to sqrt(96).
 * https://oeis.org/A041172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Numerators a(n) of continued‑fraction convergents to √96.
   a(n) = c(n) * a(n‑1) + a(n‑2),   with
   a(0)=9, a(1)=10, and coefficients
   c(0)=9,  c(1)=1,  c(2)=3,  c(3)=1,  c(4)=18,  and afterwards
   c(k)=c(k mod 4 + 1) for k≥5 (period 4: 1,3,1,18). */
seq_t a(idx_t n)
{
    /* base cases handled by the loop */
    seq_t p_prev2 = (seq_t)0;  /* a(n‑2)  */
    seq_t p_prev1 = (seq_t)1;  /* a(n‑1)  */
    seq_t coeff;
    for (idx_t i = 0; i <= n; ++i)
    {
        /* determine the coefficient a_i */
        if (i == 0)
            coeff = (seq_t)9;
        else
        {
            /* cycle 1,3,1,18 for indices 1,2,3,4 and repeat */
            switch ((i - 1) & 3)   /* (i-1)%4 but faster */
            {
                case 0: coeff = (seq_t)1;  break;
                case 1: coeff = (seq_t)3;  break;
                case 2: coeff = (seq_t)1;  break;
                default: coeff = (seq_t)18; break;
            }
        }
        /* recurrence: a_i = coeff * a_{i‑1} + a_{i‑2} */
        seq_t curr = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = curr;
    }
    return p_prev1;
}
