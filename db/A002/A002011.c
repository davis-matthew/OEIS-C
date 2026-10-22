/*
 * A002011  a(n) = 4*(2n+1)!/n!^2.
 * https://oeis.org/A002011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(2n+1)!/n!^2 = 4*(2n+1)*C(2n,n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; i++) {
        binom = binom * (seq_t)(n + i) / (seq_t)i;
    }
    
    seq_t result = (seq_t)4 * ((seq_t)2 * n + 1) * binom;
    return result;
}
