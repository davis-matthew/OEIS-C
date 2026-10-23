/*
 * A190876  Numbers 1 through 8 together with numbers congruent to 9 mod 10.
 * https://oeis.org/A190876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n for n <= 8; a(n) = 10*n - 81 for n >= 9
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= (idx_t)8) {
        return (seq_t)n;
    }
    return (seq_t)n * 10 - (seq_t)81;
}
