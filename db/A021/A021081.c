/*
 * A021081  Decimal expansion of 1/77.
 * https://oeis.org/A021081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = digit n of repeating cycle 0,1,2,9,8,7 (period 6)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t cycle[6] = {0, 1, 2, 9, 8, 7};
    idx_t r = n % 6;
    return cycle[r];
}
