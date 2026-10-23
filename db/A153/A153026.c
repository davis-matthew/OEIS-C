/*
 * A153026  a(1)=0, a(n) = n^3 - a(n-1).
 * https://oeis.org/A153026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 6*n^2 + 9*(-1)^n - 1) / 8
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;              /* n^2 */
    seq_t n3 = n2 * ns;              /* n^3 */
    seq_t num = n3 * (seq_t)4 + n2 * (seq_t)6;  /* 4n^3 + 6n^2 */

    if (n & 1)        /* odd n: subtract 9 */
        num -= (seq_t)9;
    else              /* even n: add 9 */
        num += (seq_t)9;

    num -= (seq_t)1;
    return num / (seq_t)8;
}
