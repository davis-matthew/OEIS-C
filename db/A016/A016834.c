/*
 * A016834  a(n) = (4n+2)^10.
 * https://oeis.org/A016834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+2)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 20, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;            /* convert index to seq_t first */
    x = x * (seq_t)4 + (seq_t)2;    /* compute 4n+2 */
    seq_t result = 1;
    for (int i = 0; i < 10; ++i) {
        result = result * x;        /* multiply x ten times */
    }
    return result;
}
