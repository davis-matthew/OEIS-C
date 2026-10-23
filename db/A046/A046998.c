/*
 * A046998  a(n) = 1 - (7/6)*n + (2/3)*n^3 + (1/2)*n^4.
 * https://oeis.org/A046998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 + 4*n^3 - 7*n + 6) / 6
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n) = 1 - (7/6)*n + (2/3)*n^3 + (1/2)*n^4 as an exact integer. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n2 * n2;          /* n^4 */

    seq_t num = 3 * n4;          /* 3*n^4 */
    num += 4 * n3;               /* +4*n^3 */
    num -= 7 * nn;               /* -7*n */
    num += 6;                    /* +6 */

    return num / 6;              /* numerator is always divisible by 6 */
}
