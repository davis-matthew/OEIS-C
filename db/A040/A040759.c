/*
 * A040759  Continued fraction for sqrt(788).
 * https://oeis.org/A040759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040759: continued fraction coefficients of sqrt(788).
   a(0) = 28; for n ≥ 1, a(n) = 14 if n is odd, 56 if n is even. */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)28;
    }
    return (seq_t)((n & 1) ? 14 : 56);
}
