/*
 * A010229  Continued fraction for sqrt(187).
 * https://oeis.org/A010229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence coming from the continued fraction of √187:
   a(0) = 13.
   For n > 0, a(n) repeats the block [1, 2, 13, 2, 1, 26] every 6 terms. */
seq_t a(idx_t n) {
    if (n == 0)
        return 13;
    static const seq_t cycle[6] = {1, 2, 13, 2, 1, 26};
    return cycle[(n - 1) % 6];
}
