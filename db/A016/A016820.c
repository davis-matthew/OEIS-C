/*
 * A016820  a(n) = (4*n + 1)^8.
 * https://oeis.org/A016820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 63, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)4 * (seq_t)n + 1;
    seq_t r1   = base * base;   /* base^2   */
    seq_t r2   = r1  * r1;      /* base^4   */
    seq_t r3   = r2  * r2;      /* base^8   */
    return r3;
}
