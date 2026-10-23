/*
 * A040451  Continued fraction for sqrt(473).
 * https://oeis.org/A040451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040451 – Multiplicative function:
 * a(0) = 21;
 * For n ≥ 1:
 *   if n is odd → 1,
 *   if n ≡ 2 (mod 4) → 2,
 *   if n ≡ 0 (mod 4) → 42. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)21;
    if (n & 1ULL) return (seq_t)1;
    if ((n & 3ULL) == 0) return (seq_t)42;
    return (seq_t)2;
}
