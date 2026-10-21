/*
 * A071061  Abjad values of the Arabic letters in the traditional order for
 *          abjad calculations.
 * https://oeis.org/A071061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   * If n <= 10 : a(n) = n * If 10 < n <= 18 : a(n) = 10 * (n - 9)
 *            * If n > 18 : a(n) = 100 * (n - 18) * This implements the abjad
 *            values for the Arabic letters in traditional order
 * Range:     u8 n <= 20, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)n;          /* work in seq_t arithmetic */
    if (t <= 10)
        return t;
    if (t <= 18)
        return (seq_t)(10 * (t - 9));
    return (seq_t)(100 * (t - 18));
}
