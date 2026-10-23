/*
 * A016852  a(n) = (5n)^4.
 * https://oeis.org/A016852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n)^4
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t res = (seq_t)5 * m;   /* 5n */
    res *= res;                 /* (5n)^2 */
    res *= res;                 /* (5n)^4 */
    return res;
}
