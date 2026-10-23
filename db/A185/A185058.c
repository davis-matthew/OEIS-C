/*
 * A185058  a(n) = n^n! mod 7.
 * https://oeis.org/A185058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n! mod 7 Computation uses Fermat's little theorem: for
 *            n≥3 and n%7≠0, n! ≡ 0 (mod 6), thus a(n) ≡ 1. For n%7==0 the
 *            result is 0. Small values are handled explicitly
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 4;
    if ((n % 7) == 0) return 0;
    return 1;
}
