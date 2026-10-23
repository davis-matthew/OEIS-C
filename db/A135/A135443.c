/*
 * A135443  Number of maximal directed trails in the labeled n-ladder graph
 *          P_2 X P_n.
 * https://oeis.org/A135443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 4*(n-2)*(2*n-3) for n>2; a(1)=2; a(2)=8
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;
    seq_t ns = (seq_t)n;
    return (seq_t)4 * (ns - 2) * ((seq_t)2 * ns - 3);
}
