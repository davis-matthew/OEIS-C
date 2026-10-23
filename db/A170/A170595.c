/*
 * A170595  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0; a(n) = 10 * 9^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1          */
        return (seq_t)1;

    seq_t res = (seq_t)10;          /* initial factor 10    */
    idx_t exp = n - 1;              /* exponent for 9       */
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)9;            /* multiply by 9 repeatedly */

    return res;                     /* a(n) = 10 * 9^(n-1)  */
}
