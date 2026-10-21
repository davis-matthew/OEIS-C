/*
 * A215409  The Goodstein sequence G_n(3).
 * https://oeis.org/A215409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, easy, nonn, fini, full
 * Formula:   a(n) = 3 for n = 0,1,2; 2 for n = 3; 1 for n = 4; 0 otherwise
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)3;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)1;
    return (seq_t)0;
}
