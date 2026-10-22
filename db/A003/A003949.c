/*
 * A003949  Expansion of g.f. (1+x)/(1-6*x).
 * https://oeis.org/A003949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = 7*6^(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t result = 7;
    for (idx_t i = 1; i < n; i++) {
        result *= 6;
    }
    return result;
}
