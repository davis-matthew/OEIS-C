/*
 * A041965  Denominators of continued fraction convergents to sqrt(505).
 * https://oeis.org/A041965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041965: Denominators of continued fraction convergents to sqrt(505)
   Recurrence:
   a(0) = 1
   For n ≥ 1 let
       coeff(n) = {2, 8, 2, 44}[ (n-1) mod 4 ]
   Then
       a(n) = coeff(n) • a(n-1) + a(n-2)
   (The first coefficient for n=0 is 22, i.e. a(0)=22*0+1=1.)
*/

seq_t a(idx_t n)
{
    /* periodic part of the continued fraction of sqrt(505) after the first term */
    static const int period[4] = { 2, 8, 2, 44 };

    seq_t prev2 = 1;   /* a(-2) = 1   */
    seq_t prev1 = 0;   /* a(-1) = 0   */

    for (idx_t i = 0; i <= n; ++i)
    {
        int coeff = (i == 0) ? 22 : period[(int)((i - 1) % 4)];
        seq_t cur = (seq_t)coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
