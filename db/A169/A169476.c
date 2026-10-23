/*
 * A169476  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=31*30^(n-1) for n>=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    seq_t power = 1;
    /* iterate n-1 times to compute 30^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        power *= (seq_t)30;
    return (seq_t)31 * power;
}
