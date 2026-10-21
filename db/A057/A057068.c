/*
 * A057068  a(n) = floor(6^6/n).
 * https://oeis.org/A057068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(6^6 / n) = floor(46656 / n)
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 6^6 = 46656 */
    seq_t const const_val = (seq_t)46656;
    /* n is guaranteed >= 1 */
    return const_val / (seq_t)n;
}
