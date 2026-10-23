/*
 * A120324  Periodic sequence 0, 1, 0, 4, 0, 1.
 * https://oeis.org/A120324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = a(n-6) for n > 5; initial values a(0)=0, a(1)=1, a(2)=0,
 *            a(3)=4, a(4)=0, a(5)=1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[6] = {0, 1, 0, 4, 0, 1};
    return tbl[n % 6];
}
