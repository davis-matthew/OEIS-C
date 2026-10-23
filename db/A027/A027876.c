/*
 * A027876  a(n) = Product_{i=1..n} (8^i - 1).
 * https://oeis.org/A027876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} (8^i – 1), a(0)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t pow = 1;
    for (idx_t i = 1; i <= n; ++i) {
        pow *= 8;
        seq_t term = pow - 1;
        result *= term;
    }
    return result;
}
