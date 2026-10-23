/*
 * A126696  Tenth-squares: floor(n/10)*ceiling(n/10).
 * https://oeis.org/A126696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n/10)*ceil(n/10)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t a = x / 10;          /* floor(n/10) */
    seq_t b = (x + 9) / 10;    /* ceil(n/10) */
    return a * b;
}
