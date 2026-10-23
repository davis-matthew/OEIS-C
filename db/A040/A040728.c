/*
 * A040728  Continued fraction for sqrt(756).
 * https://oeis.org/A040728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction coefficients of sqrt(756). a(0) = 27; for n ≥
 *            1, a(n) = 2 if n is odd, 54 if n is even
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)27;
    return (n & (idx_t)1) ? (seq_t)2 : (seq_t)54;
}
