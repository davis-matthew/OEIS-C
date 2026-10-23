/*
 * A040469  Continued fraction for sqrt(492).
 * https://oeis.org/A040469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction expansion of sqrt(492).
   a(0) = 22; for n > 0 the terms repeat the period
   [5, 1, 1, 10, 1, 1, 5, 44]. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)22;
    static const seq_t period[8] = {5, 1, 1, 10, 1, 1, 5, 44};
    return period[(int)((n - 1) % 8)];
}
