/*
 * A158640  a(n) = 52*n^2 - 1.
 * https://oeis.org/A158640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 9088, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 52*n^2 - 1
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               // offset is 1; n==0 is undefined
    seq_t nn = (seq_t)n;
    return 52 * nn * nn - 1;
}
