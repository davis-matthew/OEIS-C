/*
 * A070573  a(n) = n^4 mod 30.
 * https://oeis.org/A070573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 30
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = (x * x) % (seq_t)30;   /* x = n^2 mod 30 */
    x = (x * x) % (seq_t)30;   /* x = (n^2)^2 = n^4 mod 30 */
    return x;
}
