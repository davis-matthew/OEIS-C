/*
 * A144410  a(n) = 4*(3*n+1)*(3*n+2).
 * https://oeis.org/A144410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(3*n+1)*(3*n+2)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t u = 3*tn + 1;
    seq_t v = 3*tn + 2;
    return 4*u*v;
}
