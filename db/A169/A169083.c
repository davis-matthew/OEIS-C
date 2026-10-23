/*
 * A169083  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 22 * 21^(n-1) for n >= 1 (Number of reduced
 *            words of length n in the Coxeter group described with 22
 *            generators and relations (S_i)^2 = (S_i S_j)^25 = I)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)22;
    /* Compute 21^(n-1) by repeated multiplication */
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)21;

    return result;
}
