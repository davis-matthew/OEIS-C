/*
 * A006501  Expansion of (1+x^2) / ( (1-x)^2 * (1-x^3)^2 ).
 * https://oeis.org/A006501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+3)/3) * floor((n+4)/3) * floor((n+5)/3)
 * Range:     u8 n <= 16, u16 n <= 117, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t f1 = (n_seq + 3) / 3;
    seq_t f2 = (n_seq + 4) / 3;
    seq_t f3 = (n_seq + 5) / 3;
    return f1 * f2 * f3;
}
