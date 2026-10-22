/*
 * A006472  a(n) = n!*(n-1)!/2^(n-1).
 * https://oeis.org/A006472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n!*(n-1)!/2^(n-1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t tri = ((seq_t)(i - 1) * (seq_t)i) / 2;
        result = result * tri;
    }
    return result;
}
