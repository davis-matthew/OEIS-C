/*
 * A016889  a(n) = (5n+3)^5.
 * https://oeis.org/A016889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 3)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 16, u64 n <= 1425,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)n) * (seq_t)5 + (seq_t)3; /* 5n + 3 */
    seq_t res = m;          /* start with one factor of (5n+3) */
    res *= m;               /* (5n+3)^2 */
    res *= m;               /* (5n+3)^3 */
    res *= m;               /* (5n+3)^4 */
    res *= m;               /* (5n+3)^5 */
    return res;
}
