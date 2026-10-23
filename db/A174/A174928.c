/*
 * A174928  Partial sums of A174927.
 * https://oeis.org/A174928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (67 + 130*n - 63*(-1)^n)/4
 * Range:     u8 n <= 0, u16 n <= 733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term = 130*sn + 67;
    if ((n & 1) == 0) {
        term -= 63;
    } else {
        term += 63;
    }
    return term / 4;
}
