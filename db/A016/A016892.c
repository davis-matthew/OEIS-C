/*
 * A016892  a(n) = (5n+3)^8.
 * https://oeis.org/A016892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n+3)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    x = x * 5 + 3;       /* x = 5n + 3 */
    seq_t res = x;
    for (idx_t i = 1; i < 8; ++i)
        res *= x;         /* res = x^8 */
    return res;
}
