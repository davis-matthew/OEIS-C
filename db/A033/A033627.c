/*
 * A033627  0-additive sequence: not the sum of any previous pair.
 * https://oeis.org/A033627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 5 for n>2, with a(1)=1 and a(2)=2
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    return ((seq_t)3 * (seq_t)n) - (seq_t)5;
}
