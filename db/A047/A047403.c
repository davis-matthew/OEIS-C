/*
 * A047403  Numbers that are congruent to {0, 2, 3, 6} mod 8.
 * https://oeis.org/A047403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1) - ((n-1) mod 4 == 2) (indices start at 1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t val = (seq_t)m * 2;
    if ((m % 4) == 2) val -= 1;
    return val;
}
