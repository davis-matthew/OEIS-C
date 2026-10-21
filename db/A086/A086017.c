/*
 * A086017  Number of 9's in decimal expansion of n^2.
 * https://oeis.org/A086017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of '9' digits in the decimal representation of n^2
 * Range:     u8 n <= 16, u16 n <= 306, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;   /* compute n^2 in seq_t */
    seq_t count = 0;
    seq_t tmp = sq;

    /* Special case: 0^2 = 0 has no digit '9' */
    if (tmp == 0) {
        return count;
    }

    while (tmp > 0) {
        if ((tmp % 10) == 9) {
            ++count;
        }
        tmp /= 10;
    }

    return count;
}
