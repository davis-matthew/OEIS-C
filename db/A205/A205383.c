/*
 * A205383  a(n) = (1/n)*A205382(n).
 * https://oeis.org/A205383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 / gcd(8,n) = 8 >> min(ctz(n),3)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tz = 0;
    while ((n & 1ULL) == 0 && tz < 3) {
        tz++;
        n >>= 1;
    }
    return ((seq_t)8) >> tz;
}
