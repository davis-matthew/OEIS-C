/*
 * A081594  Let n = 10x + y where 0 <= y <= 9, x >= 0. Then a(n) = 2x+y.
 * https://oeis.org/A081594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/10) + n mod 10
 * Range:     u8 n <= 1247, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)(n / 10);
    seq_t y = (seq_t)(n % 10);
    return 2 * x + y;
}
