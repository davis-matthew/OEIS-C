/*
 * A006478  a(n) = a(n-1) + a(n-2) + F(n) - 1, a(0) = a(1) = 0, where F() =
 *          Fibonacci numbers A000045.
 * https://oeis.org/A006478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + F(n) - 1, a(0) = a(1) = 0, where F(n) =
 *            Fibonacci numbers A000045
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 85, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 0;   /* a(i-2) */
    seq_t y = 0;   /* a(i-1) */
    seq_t fp = 0;  /* F(i-1) */
    seq_t fc = 1;  /* F(i)   */
    idx_t i;
    if (n < 2) return 0;
    for (i = 2; i <= n; i++) {
        seq_t t = fp + fc;
        seq_t z;
        fp = fc;
        fc = t;
        z = y + x + fc - 1;
        x = y;
        y = z;
    }
    return y;
}
