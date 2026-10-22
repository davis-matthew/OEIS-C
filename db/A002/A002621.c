/*
 * A002621  Expansion of 1 / ((1-x)^2*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A002621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(9*(-1)^n + n^3 + 21*n^2 + 145*n + 350)/576 +
 *            1/2)
 * Range:     u8 none, u16 n <= 33, u32 n <= 1038, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    
    seq_t sign = (n & 1) ? -1 : 1;
    seq_t poly = 9 * sign + n3 + 21 * n2 + 145 * n_seq + 350;
    
    seq_t result = ((n_seq + 1) * poly + 288) / 576;
    return result;
}
