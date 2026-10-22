/*
 * A002624  Expansion of (1-x)^(-3) * (1-x^2)^(-2).
 * https://oeis.org/A002624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*(2*n + 7)*(-1)^n + 2*n^4 + 28*n^3 + 136*n^2 + 266*n +
 *            171) / 192
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 368, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t n4 = n3 * ns;
    
    seq_t poly = (seq_t)2 * n4 + (seq_t)28 * n3 + (seq_t)136 * n2 + (seq_t)266 * ns + (seq_t)171;
    
    seq_t alt_term = (seq_t)3 * ((seq_t)2 * ns + (seq_t)7);
    if (n & 1) {
        poly -= alt_term;
    } else {
        poly += alt_term;
    }
    
    return poly / (seq_t)192;
}
