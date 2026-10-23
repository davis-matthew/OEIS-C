/*
 * A039823  a(n) = ceiling((n^2 + n + 2)/4).
 * https://oeis.org/A039823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n^2 + n + 2)/4)
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;                  /* convert index to seq_t */
    seq_t num = x * x + x + (seq_t)2;    /* n^2 + n + 2   */
    return (num + (seq_t)3) / (seq_t)4;   /* ceil division by 4 */
}
