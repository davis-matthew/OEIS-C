/*
 * A017237  a(n) = (9*n + 6)^5.
 * https://oeis.org/A017237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 6)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 8, u64 n <= 791, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base: 9*n + 6 */
    seq_t base = (seq_t)9 * n + 6;

    /* Raise the base to the 5th power */
    seq_t res = base;        /* base^1 */
    res *= base; /* base^2 */
    res *= base; /* base^3 */
    res *= base; /* base^4 */
    res *= base; /* base^5 */

    return res;
}
