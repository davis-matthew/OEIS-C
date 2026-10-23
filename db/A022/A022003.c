/*
 * A022003  Decimal expansion of 1/999.
 * https://oeis.org/A022003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ( (n+1) mod 3 == 0 ) ? 1 : 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem = (n + 1) % 3;
    return rem == 0 ? (seq_t)1 : (seq_t)0;
}
