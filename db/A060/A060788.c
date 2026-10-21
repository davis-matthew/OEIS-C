/*
 * A060788  a(n) = 9*(n-2)^2 * (n^2 - 2*n - 1).
 * https://oeis.org/A060788
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*(n-2)^2 * (n^2 - 2*n - 1) (offset: 2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 149, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the integer type used for the sequence */
    seq_t sn = n;

    /* (n - 2)^2 */
    seq_t t1   = sn - (seq_t)2;
    seq_t t1sq = t1 * t1;

    /* (n^2 - 2*n - 1) */
    seq_t t2 = sn * sn - (seq_t)2 * sn - (seq_t)1;

    /* Final result */
    return (seq_t)9 * t1sq * t2;
}
