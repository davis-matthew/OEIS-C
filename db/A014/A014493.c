/*
 * A014493  Odd triangular numbers.
 * https://oeis.org/A014493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(2*n-1-(-1)^n)/2
 * Range:     u8 n <= 12, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based index; compute odd triangular numbers */
    seq_t nn   = (seq_t)n;          /* convert index to seq_t */
    seq_t m    = (nn << 1) - 1;     /* 2*n - 1 */
    seq_t temp = (n & 1) ? (m + 1) : (m - 1);  /* m - (-1)^n */
    return (m * temp) / 2;         /* final result */
}
