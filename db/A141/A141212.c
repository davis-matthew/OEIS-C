/*
 * A141212  a(n) = 1, if n == {1,3,4} mod 6; otherwise 0.
 * https://oeis.org/A141212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n == {1,3,4} mod 6, otherwise 0
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned rem = (unsigned)(n % 6);
    return (rem == 1 || rem == 3 || rem == 4) ? (seq_t)1 : (seq_t)0;
}
