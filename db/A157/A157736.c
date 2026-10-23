/*
 * A157736  a(n) = 388962*n^2 - 347508*n + 77617.
 * https://oeis.org/A157736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 388962*n^2 - 347508*n + 77617
 * Range:     u8 none, u16 none, u32 n <= 105, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;          /* n^2 */
    res *= 388962;                /* 388962*n^2 */
    res -= 347508 * nn;           /* subtract 347508*n */
    res += 77617;                 /* add constant */
    return res;
}
