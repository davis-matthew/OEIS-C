/*
 * A084253  a(n) is the denominator of the coefficient of z^(2n-1) in the
 *          Maclaurin expansion of Sqrt[Pi]Erfi[z].
 * https://oeis.org/A084253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(n-1)!/2 for n>2; a(1)=1, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;

    seq_t result = 1;
    for (idx_t i = 2; i < n; ++i)
        result *= (seq_t)i;          /* compute (n-1)! */

    seq_t odd_factor = ((seq_t)2 * (seq_t)n) - (seq_t)1; /* 2*n-1 */
    result *= odd_factor;
    result /= 2;                     /* divide by 2 */

    return result;
}
