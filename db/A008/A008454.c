/*
 * A008454  Tenth powers: a(n) = n^10.
 * https://oeis.org/A008454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t res = (seq_t)1;
    for (int i = 0; i < 10; ++i)
        res *= x;
    return res;
}
