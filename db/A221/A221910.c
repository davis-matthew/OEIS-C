/*
 * A221910  a(n) = 8^n + 8*n.
 * https://oeis.org/A221910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n + 8*n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute 8^n */
    seq_t val = 1;
    idx_t k = n;
    while (k--) {
        val *= (seq_t)8;
    }
    /* add 8*n */
    val += (seq_t)8 * (seq_t)n;
    return val;
}
