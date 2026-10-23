/*
 * A166138  Trisection A022998(3n+1).
 * https://oeis.org/A166138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)*(3-(-1)^n)/2
 * Range:     u8 n <= 42, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;
    if ((n & 1u) == 0u) {
        return 3*sn + 1;
    } else {
        return 6*sn + 2;
    }
}
