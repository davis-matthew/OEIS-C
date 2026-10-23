/*
 * A024177  a(n) = floor ( (2nd elementary symmetric function of
 *          2,3,...,n+2)/(2+3+...+n+2) ).
 * https://oeis.org/A024177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n*(3 n^2 + 23 n + 46) / (12*(n + 4)) )
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 1351, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t part = (seq_t)3 * n2 + (seq_t)23 * N + (seq_t)46;
    seq_t num  = N * part;
    seq_t den  = (seq_t)12 * (N + (seq_t)4);
    return num / den;
}
