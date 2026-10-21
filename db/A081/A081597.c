/*
 * A081597  Let n = 10*x + y where 0 <= y <= 9, x >= 0. Then a(n) = 6*x + y.
 * https://oeis.org/A081597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 4*floor(n/10) (equivalently a(10*x + y) = 6*x + y)
 * Range:     u8 n <= 423, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n - (seq_t)(4 * (n / 10));
}
