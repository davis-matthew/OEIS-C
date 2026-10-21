/*
 * A058796  Row 5 of A007754.
 * https://oeis.org/A058796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + 15*n^4 + 81*n^3 + 189*n^2 + 173*n + 33
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 81, u64 n <= 7128,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type */
    seq_t n_seq = (seq_t)n;
    /* Horner's method for efficient evaluation */
    seq_t t = (seq_t)1;
    t = t * n_seq + (seq_t)15;
    t = t * n_seq + (seq_t)81;
    t = t * n_seq + (seq_t)189;
    t = t * n_seq + (seq_t)173;
    t = t * n_seq + (seq_t)33;
    return t;
}
