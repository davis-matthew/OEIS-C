/*
 * A210032  a(n)=n for n=1,2,3 and 4; a(n)=5 for n >= 5.
 * https://oeis.org/A210032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min(n,5)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return n for n=1..4, otherwise 5 */
    return (n < 5) ? (seq_t)n : (seq_t)5;
}
