/*
 * A181427  a(n) = n + [n^2 if n is odd or n^3 if n is even].
 * https://oeis.org/A181427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + n^2 if n odd, n + n^3 if n even
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (n & 1ULL) {
        return nn + nn * nn;
    } else {
        seq_t sq = nn * nn;
        return nn + sq * nn;
    }
}
