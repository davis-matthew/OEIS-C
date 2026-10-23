/*
 * A030104  Base 5 reversal of n (written in base 10).
 * https://oeis.org/A030104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = the integer obtained by reversing the base‑5 digits of n
 *            (and interpreting the reversed digits as base‑5)
 * Range:     u8 n <= 127, u16 n <= 3124, u32 n <= 3124, u64 n <= 3124,
 *            u128 n <= 3124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 0;
    idx_t x = n;
    while (x > 0) {
        seq_t digit = (seq_t)(x % 5);
        x /= 5;
        res = res * 5 + digit;
    }
    return res;
}
