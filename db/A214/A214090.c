/*
 * A214090  Period 6: repeat [0, 0, 1, 0, 1, 1].
 * https://oeis.org/A214090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {0,0,1,0,1,1}[n mod 6]
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    return ((r==2) || (r==4) || (r==5)) ? (seq_t)1 : (seq_t)0;
}
