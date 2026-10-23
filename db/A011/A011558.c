/*
 * A011558  Expansion of (x + x^3)/(1 + x + ... + x^4) mod 2.
 * https://oeis.org/A011558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^4 mod 5 (characteristic function of numbers coprime to
 *            5)
 * Range:     u8 n <= 9126, u16 n <= 9126, u32 n <= 9126, u64 n <= 9126,
 *            u128 n <= 9126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The value is 0 iff n is a multiple of 5, otherwise 1. */
    return (seq_t)((n % 5 == 0) ? 0 : 1);
}
