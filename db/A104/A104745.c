/*
 * A104745  a(n) = 5^n + n.
 * https://oeis.org/A104745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n + n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;          /* start with 5^0 = 1 */
    for (idx_t i = 0; i < n; ++i) {
        result *= (seq_t)5;    /* multiply by 5 each step */
    }
    result += (seq_t)n;        /* add n to get 5^n + n */
    return result;
}
