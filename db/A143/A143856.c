/*
 * A143856  Ulam's spiral (ENE spoke).
 * https://oeis.org/A143856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 - 37*n + 22
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;           /* convert index to seq_t */
    seq_t res = 16 * nn * nn;   /* 16*n^2 */
    res -= 37 * nn;             /* -37*n */
    res += 22;                  /* +22 */
    return res;
}
