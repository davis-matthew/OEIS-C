/*
 * A001932  Sum of Fibonacci (A000045) and Pell (A000129) numbers.
 * https://oeis.org/A001932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n) + P(n), where F(n) = F(n-1) + F(n-2), F(0) = 0, F(1)
 *            = 1 (Fibonacci) and P(n) = 2*P(n-1) + P(n-2), P(0) = 0, P(1) = 1
 *            (Pell)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t f0 = 0, f1 = 1;
    seq_t p0 = 0, p1 = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t fn = f0 + f1;
        seq_t pn = p1 + p1 + p0;
        f0 = f1;
        f1 = fn;
        p0 = p1;
        p1 = pn;
    }
    return f0 + p0;
}
