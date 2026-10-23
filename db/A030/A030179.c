/*
 * A030179  Quarter-squares squared: A002620^2.
 * https://oeis.org/A030179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n^2 / 4))^2
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work in the destination integer type to keep intermediate
       results exact.  For any size of seq_t (8, 16, 32, 64, 128
       bits or arbitrary precision) the following arithmetic is
       sufficient and fast. */
    seq_t sn = n;          /* convert index to seq_t */
    seq_t sq = sn * sn;    /* n^2 */
    seq_t r  = sq / 4;     /* floor(n^2/4) */
    return r * r;          /* square the result */
}
