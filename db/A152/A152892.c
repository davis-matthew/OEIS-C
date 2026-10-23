/*
 * A152892  Period 5: repeat [0, 3, 1, 0, 1].
 * https://oeis.org/A152892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3 + 2*n^2) mod 5
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r  = n % 5;          /* n mod 5 */
    seq_t r2 = (r * r) % 5;    /* r^2 mod 5 */
    seq_t r3 = (r2 * r) % 5;   /* r^3 mod 5 */
    seq_t s  = (2 * r2) % 5;   /* 2*r^2 mod 5 */
    return (r3 + s) % 5;       /* (r^3 + 2*r^2) mod 5 */
}
