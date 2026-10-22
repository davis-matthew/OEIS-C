/*
 * A007696  Quartic (or 4-fold) factorial numbers: a(n) = Product_{k = 0..n-1}
 *          (4*k + 1).
 * https://oeis.org/A007696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} (4*k + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 0; k < n; k++) {
        result *= (seq_t)(4 * k + 1);
    }
    return result;
}
