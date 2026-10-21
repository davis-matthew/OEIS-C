/*
 * A081585  Third row of Pascal-(1,3,1) array A081578.
 * https://oeis.org/A081585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n^2 + 1
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t for exact arithmetic */
    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * sn;
    return (seq_t)8 * sn2 + (seq_t)1;
}
