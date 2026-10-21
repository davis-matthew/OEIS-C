/*
 * A050492  Thickened cube numbers: a(n) = n*(n^2 + (n-1)^2) +
 *          (n-1)*2*n*(n-1).
 * https://oeis.org/A050492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ((2*n-1)^3 + 1)/2
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for accurate arithmetic */
    seq_t tn = n;
    seq_t t = 2 * tn - 1;
    seq_t t3 = t * t * t;
    return (t3 + 1) / 2;
}
