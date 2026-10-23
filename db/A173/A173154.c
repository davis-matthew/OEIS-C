/*
 * A173154  a(n) = n^3/6 + 3*n^2/4 + 7*n/3 + 7/8 + (-1)^n/8.
 * https://oeis.org/A173154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3/6 + 3*n^2/4 + 7*n/3 + 7/8 + (-1)^n/8
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1885, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for arithmetic */
    seq_t t = (seq_t)n;
    seq_t t2 = t * t;          /* n^2   */
    seq_t t3 = t2 * t;         /* n^3   */

    /* numerator = 4*n^3 + 18*n^2 + 56*n + 21 + 3*(-1)^n */
    seq_t num = 4 * t3 + 18 * t2 + 56 * t + 21;

    /* add or subtract 3 depending on parity of n */
    if ((n & 1) == 0)          /* even n -> (-1)^n = +1 */
        num += 3;
    else                       /* odd  n -> (-1)^n = -1 */
        num -= 3;

    /* division by 24 is exact for all n */
    return num / 24;
}
