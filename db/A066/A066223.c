/*
 * A066223  Bisection of A000085.
 * https://oeis.org/A066223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2. For n >= 2: a(n) = 2*(2*n-1)*a(n-1) -
 *            2*(n-1)*(2*n-3)*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n == 1)
        return (seq_t)2;

    seq_t a_prev2 = (seq_t)1;      /* a(0) */
    seq_t a_prev1 = (seq_t)2;      /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t coeff1 = ((seq_t)2) * ((seq_t)(2 * i - 1));
        seq_t term1  = coeff1 * a_prev1;

        seq_t coeff2 = ((seq_t)2) * ((seq_t)(i - 1)) * ((seq_t)(2 * i - 3));
        seq_t term2  = coeff2 * a_prev2;

        a_curr = term1 - term2;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
