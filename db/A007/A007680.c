/*
 * A007680  a(n) = (2n+1)*n!.
 * https://oeis.org/A007680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n+1)*n!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        result *= (seq_t)i;
    }
    return result * ((seq_t)n * 2 + 1);
}
