/*
 * A169567  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=26, a(2)=650, a(n)=650*25^(n-2) for n>=2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;

    /* compute 25^(n-2) iteratively */
    seq_t pow = (seq_t)1;
    idx_t i = n - 2;
    while (i--) {
        pow *= (seq_t)25;
    }
    return (seq_t)650 * pow;
}
