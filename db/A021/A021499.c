/*
 * A021499  Decimal expansion of 1/495.
 * https://oeis.org/A021499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 2 if n >= 2 and n is even, else a(n) = 0
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    return ((n & 1) == 0) ? (seq_t)2 : (seq_t)0;
}
