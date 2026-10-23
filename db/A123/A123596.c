/*
 * A123596  Squares alternating with triangular numbers.
 * https://oeis.org/A123596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = n^2 a(2*n+1) = (n^2 + n)/2 (Same as a(n) =
 *            (3*n^2-1+(n^2+1)*(-1)^n)/16)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use integer arithmetic only. */
    if ((n & 1) == 0)               /* even */
    {
        seq_t k = (seq_t)(n >> 1);  /* k = n/2 */
        return k * k;
    }
    else                            /* odd */
    {
        seq_t k = (seq_t)(n >> 1);  /* k = (n-1)/2 */
        return (k * k + k) / 2;
    }
}
