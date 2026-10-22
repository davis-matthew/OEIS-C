/*
 * A000425  Coefficients of ménage hit polynomials.
 * https://oeis.org/A000425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*b(n), where b(n) = A000271(n) satisfies b(n) =
 *            (n-2)*b(n-1) + (n-2)*b(n-2) + b(n-3), b(1) = 1, b(2) = 0, b(3) =
 *            0 (this follows from dividing the conjectured recurrence for
 *            a(n) by 2n(n-1)(n-3))
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t b3 = 1, b2 = 0, b1 = 0, b = 0;
    idx_t i;
    if (n == 1) return 2;
    if (n == 2 || n == 3) return 0;
    for (i = 4; i <= n; i++) {
        seq_t m = (seq_t)(i - 2);
        b = m * b1 + m * b2 + b3;
        b3 = b2;
        b2 = b1;
        b1 = b;
    }
    return (seq_t)2 * (seq_t)n * b;
}
