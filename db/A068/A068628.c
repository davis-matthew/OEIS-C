/*
 * A068628  Numbers occurring twice in A068627.
 * https://oeis.org/A068628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12 * (4*n - 3) * (n - 1)
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 9460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)4 * nn - (seq_t)3;
    seq_t term2 = nn - (seq_t)1;
    return (seq_t)12 * term1 * term2;
}
