/*
 * A024201  [ (2nd elementary symmetric function of S(n))/(first elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+1 odd
 *          positive integers}.
 * https://oeis.org/A024201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n - 2)/2 - floor(n/6)
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at n = 1 */
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn + nn - 2;         /* n^2 + n - 2 */
    val = val / 2;                        /* divide by 2 */
    seq_t sub = (seq_t)(n / 6);           /* floor(n/6) */
    val -= sub;
    return val;
}
