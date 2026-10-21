/*
 * A086010  Number of 2's in decimal expansion of n^2.
 * https://oeis.org/A086010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digit '2's in the decimal
 *            representation of n^2
 * Range:     u8 n <= 15, u16 n <= 335, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;   /* compute n^2 */
    seq_t count = 0;

    /* count digit '2' in decimal expansion of sq */
    while (sq > 0) {
        if ((sq % 10) == 2)
            ++count;
        sq /= 10;
    }
    return count;
}
