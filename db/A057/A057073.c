/*
 * A057073  a(n) = floor(11^11/n).
 * https://oeis.org/A057073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(11^11 / n)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 11^11 = 285,311,670,611 */
    const seq_t NUM = (seq_t)285311670611ULL;
    return NUM / (seq_t)n;
}
