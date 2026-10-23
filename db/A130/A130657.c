/*
 * A130657  Period 7: repeat 1,1,1,1,0,0,0.
 * https://oeis.org/A130657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n mod 7 < 4 else 0
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 7ULL < 4ULL) ? (seq_t)1 : (seq_t)0;
}
