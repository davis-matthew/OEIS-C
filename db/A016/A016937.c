/*
 * A016937  a(n) = (6*n + 2)^5.
 * https://oeis.org/A016937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 2)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 13, u64 n <= 1188,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * (seq_t)n + 2;
    seq_t res = base;
    res *= base;          /* base^2 */
    res *= base;          /* base^3 */
    res *= base;          /* base^4 */
    res *= base;          /* base^5 */
    return res;
}
