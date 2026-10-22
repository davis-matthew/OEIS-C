/*
 * A002671  a(n) = 4^n*(2*n+1)!.
 * https://oeis.org/A002671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(2*n+1)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        result *= 4;
        result *= (seq_t)(2 * k);
        result *= (seq_t)(2 * k + 1);
    }
    return result;
}
