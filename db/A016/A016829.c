/*
 * A016829  a(n) = (4n+2)^5.
 * https://oeis.org/A016829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 2)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 4*n + 2 */
    seq_t base = (seq_t)n * 4 + 2;

    /* compute base^5 */
    seq_t r = base;
    r *= base; /* base^2 */
    r *= base; /* base^3 */
    r *= base; /* base^4 */
    r *= base; /* base^5 */

    return r;
}
