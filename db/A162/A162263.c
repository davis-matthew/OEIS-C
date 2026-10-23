/*
 * A162263  a(n) = (2*n^3 + 5*n^2 + 11*n)/2.
 * https://oeis.org/A162263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 + 11*n)/2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Cast the index to seq_t to avoid accidental integer overflow. */
    seq_t nn = (seq_t)n;
    seq_t n2  = nn * nn;            /* n^2 */
    seq_t n3  = n2 * nn;            /* n^3 */
    seq_t term1 = (seq_t)2 * n3;     /* 2*n^3 */
    seq_t term2 = (seq_t)5 * n2;     /* 5*n^2 */
    seq_t term3 = (seq_t)11 * nn;    /* 11*n */
    seq_t sum   = term1 + term2 + term3;
    return sum / (seq_t)2;          /* exact division by 2 */
}
