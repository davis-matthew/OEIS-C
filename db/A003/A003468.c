/*
 * A003468  Number of minimal 3-covers of a labeled n-set.
 * https://oeis.org/A003468
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. x^3/((1-4x)(1-5x)(1-6x)(1-7x)), so a(n) = h(n-3) where
 *            c4(m) = 4^m, c5(m) = 5*c5(m-1) + c4(m), c6(m) = 6*c6(m-1) +
 *            c5(m), c7(m) = 7*c7(m-1) + c6(m), all starting at 1 for m = 0;
 *            a(n) = c7(n-3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c4 = 1, c5 = 1, c6 = 1, c7 = 1;
    idx_t m;
    if (n < 3) return 0;
    for (m = 1; m <= n - 3; m++) {
        c4 = c4 * 4;
        c5 = c5 * 5 + c4;
        c6 = c6 * 6 + c5;
        c7 = c7 * 7 + c6;
    }
    return c7;
}
