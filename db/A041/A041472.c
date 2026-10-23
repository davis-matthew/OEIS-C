/*
 * A041472  Numerators of continued fraction convergents to sqrt(252).
 * https://oeis.org/A041472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(252).
   Recurrence:
     a(0) = 15
     a(1) = 16
     a(n) = coeff(n) * a(n-1) + a(n-2)  for n >= 2
   coeff(0) = 15
   For n >= 1, coeff(n) repeats the pattern [1, 6, 1, 30]:
     coeff(1)=1, coeff(2)=6, coeff(3)=1, coeff(4)=30,
     coeff(5)=1, coeff(6)=6, ...
   This implements the standard recurrence for continued‑fraction
   convergents of sqrt(252). */

seq_t a(idx_t n)
{
    seq_t prev2 = (seq_t)0;   /* p_{-2} */
    seq_t prev1 = (seq_t)1;   /* p_{-1} */
    seq_t cur   = (seq_t)0;   /* current p_k */

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        seq_t coeff;
        if (k == (idx_t)0)
            coeff = (seq_t)15;
        else
        {
            /* pattern length 4: 1, 6, 1, 30 */
            switch ((k - (idx_t)1) % (idx_t)4)
            {
                case 0: coeff = (seq_t)1;  break;
                case 1: coeff = (seq_t)6;  break;
                case 2: coeff = (seq_t)1;  break;
                case 3: coeff = (seq_t)30; break;
            }
        }

        cur = coeff * prev1 + prev2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
