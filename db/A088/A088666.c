/*
 * A088666  a(n) = (n^4 + 1) mod 10.
 * https://oeis.org/A088666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 + 1) mod 10
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n modulo 10 is enough, because the result depends only on the last digit */
    idx_t mod10 = n % 10;
    seq_t r = (seq_t)mod10;      /* r < 10 */
    seq_t r2 = (r * r) % 10;     /* r^2 mod 10 */
    seq_t r4 = (r2 * r2) % 10;   /* r^4 mod 10 */
    return (r4 + 1) % 10;
}
