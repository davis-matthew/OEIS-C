/*
 * A228398  The number of permutations of length n sortable by 3 prefix
 *          reversals (in the pancake sorting sense).
 * https://oeis.org/A228398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n^2-3*n+3) ; a(1)=1, a(2)=2
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t sn   = (seq_t)n;
    seq_t term = sn * sn - (seq_t)3 * sn + (seq_t)3;
    return (sn - (seq_t)1) * term;
}
