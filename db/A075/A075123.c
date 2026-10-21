/*
 * A075123  a(n) is the least positive integer > a(n-1) and a(n) is not
 *          2*a(i)+a(j) for 1<=i<j<n.
 * https://oeis.org/A075123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 10 - (n mod 2) for n > 3; a(1) = 1, a(2) = 2, a(3)
 *            = 3
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;
    if (n == 3u) return (seq_t)3;

    seq_t tn = (seq_t)n;          /* convert index to seq_t */
    seq_t val = tn * (seq_t)4 - (seq_t)10;
    if (n & (idx_t)1)            /* subtract 1 if n is odd */
        val -= (seq_t)1;
    return val;
}
