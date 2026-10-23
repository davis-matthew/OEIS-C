/*
 * A008545  Quadruple factorial numbers: Product_{k=0..n-1} (4*k + 3).
 * https://oeis.org/A008545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = product_{k=0..n-1} (4*k + 3) Recurrence: a(0) = 1, a(n) =
 *            (4*n - 1) * a(n-1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)(4 * i - 1);
    }
    return res;
}
