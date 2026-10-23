/*
 * A184674  a(n) = n+floor((n/2-1/(2*n))^2); complement of A184675.
 * https://oeis.org/A184674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n/2 - 1/(2*n))^2) Equivalent closed form for n
 *            > 1: a(n) = (2*n^2 + 8*n - 9 + (-1)^n)/8 and a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 356, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t tn = (seq_t)n;
    /* Compute numerator: 2*n^2 + 8*n - 9 + (-1)^n  */
    seq_t num = (seq_t)2 * tn * tn + (seq_t)8 * tn - (seq_t)9;
    if (n & 1)       /* n is odd => (-1)^n = -1 */
        num -= (seq_t)1;
    else              /* n is even => (-1)^n = +1 */
        num += (seq_t)1;

    return num / (seq_t)8;
}
