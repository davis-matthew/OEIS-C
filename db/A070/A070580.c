/*
 * A070580  a(n) = n^4 mod 37.
 * https://oeis.org/A070580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 37
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n % (seq_t)37;
    r = (r * r) % (seq_t)37;   /* r = n^2 mod 37 */
    r = (r * r) % (seq_t)37;   /* r = (n^2)^2 mod 37 = n^4 mod 37 */
    return r;
}
