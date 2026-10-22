/*
 * A006882  Double factorials n!!: a(n) = n*a(n-2) for n > 1, a(0) = a(1) = 1.
 * https://oeis.org/A006882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = n * a(n-2) for n > 1; a(0) = 1, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 33, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    
    seq_t result = 1;
    for (idx_t i = n; i > 1; i -= 2) {
        result *= (seq_t)i;
    }
    return result;
}
