/*
 * A170423  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 30 * 29^(n-1) for n >= 1 (Number of reduced
 *            words of length n in the Coxeter group with 30 generators and
 *            relations S_i^2 = (S_i S_j)^44 = I.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                  /* a(0) = 1 */

    /* compute 29^(n-1) iteratively */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        power *= (seq_t)29;

    /* multiply by 30 to get the term */
    return power * (seq_t)30;
}
