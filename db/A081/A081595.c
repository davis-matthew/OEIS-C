/*
 * A081595  Let n = 10x + y where 0 <= y <= 9, x >= 0. Then a(n) = 4x+y.
 * https://oeis.org/A081595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 6 * floor(n/10)
 * Range:     u8 n <= 627, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n / 10;
    idx_t y = n % 10;
    return ((seq_t)x) * 4 + (seq_t)y;
}
