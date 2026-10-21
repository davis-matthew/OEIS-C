/*
 * A069816  a(n) = (sum of digits of n)^2 - (sum of digits^2 of n).
 * https://oeis.org/A069816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (sum of digits of n)^2 - (sum of squares of digits of n)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t sumsq = 0;
    idx_t t = n;

    while (t > 0) {
        idx_t d = t % 10;
        seq_t sd = (seq_t)d;
        sum += sd;
        sumsq += sd * sd;
        t /= 10;
    }

    return sum * sum - sumsq;
}
