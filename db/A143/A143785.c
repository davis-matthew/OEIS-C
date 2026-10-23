/*
 * A143785  Antidiagonal sums of the triangle A120070.
 * https://oeis.org/A143785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n even: (n^3 + 4*n^2 + 4*n)/4 if n odd : (n^3 + 4*n^2
 *            + 5*n + 2)/4 where n is the index (offset 1)
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 2577, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Cast idx_t to seq_t for arithmetic. */
    seq_t sn = (seq_t)n;

    seq_t num;
    if ((n & 1ULL) == 0) {                /* n even */
        num = sn * sn * sn + 4 * sn * sn + 4 * sn;
    } else {                              /* n odd  */
        num = sn * sn * sn + 4 * sn * sn + 5 * sn + 2;
    }
    return num / 4;
}
