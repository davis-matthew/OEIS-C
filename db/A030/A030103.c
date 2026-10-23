/*
 * A030103  Base 4 reversal of n (written in base 10).
 * https://oeis.org/A030103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = the integer obtained by reversing the base‑4
 *            representation of n, then interpreting the reversed digits in
 *            base 4 (i.e., evaluating the reversed digit sequence as a base‑4
 *            number)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 0;
    while (n > 0) {
        seq_t d = (seq_t)(n % 4);
        res = res * 4 + d;
        n /= 4;
    }
    return res;
}
