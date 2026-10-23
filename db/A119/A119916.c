/*
 * A119916  Number of runs of 0's of odd length in all ternary words of length
 *          n.
 * https://oeis.org/A119916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^(n-1)+(3+4*n)*3^(n-1))/8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute 3^(n-1) */
    seq_t pow3 = 1;
    for (idx_t i = 1; i < n; ++i)
        pow3 *= (seq_t)3;

    /* (3 + 4*n) * 3^(n-1) */
    seq_t term = pow3 * ( (seq_t)(3 + 4 * n) );

    /* Add (-1)^(n-1) : +1 if n is odd, –1 if n is even */
    seq_t num;
    if ((n & 1) == 0)          /* n even => exponent n-1 odd => -1 */
        num = term - (seq_t)1;
    else
        num = term + (seq_t)1;

    /* Division by 8 */
    return num / (seq_t)8;
}
