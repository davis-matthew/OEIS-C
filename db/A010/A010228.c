/*
 * A010228  Continued fraction for sqrt(186).
 * https://oeis.org/A010228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 13; for n > 0, a(n) = period[(n-1) % 10] with period =
 *            {1, 1, 1, 3, 4, 3, 1, 1, 1, 26}
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t period[10] = {
        (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)3, (seq_t)4, (seq_t)3,
        (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)26
    };
    if (n == 0) return (seq_t)13;
    idx_t k = (n - 1) % 10;
    return period[k];
}
