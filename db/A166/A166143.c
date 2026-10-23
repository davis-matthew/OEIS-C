/*
 * A166143  a(n) = 3*n^2 + 3*n - 5.
 * https://oeis.org/A166143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 3*n - 5
 * Range:     u8 n <= 8, u16 n <= 147, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t   = (seq_t)n;
    seq_t res = 3 * t * t;    /* 3*n^2 */
    res += 3 * t;             /* +3*n */
    res -= 5;                 /* -5   */
    return res;
}
