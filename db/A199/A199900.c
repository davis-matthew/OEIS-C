/*
 * A199900  Number of -n..n arrays x(0..4) of 5 elements with zero sum, and
 *          adjacent elements not both strictly positive and not both strictly
 *          negative.
 * https://oeis.org/A199900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (11*n^4 + 98*n^3 + 193*n^2 + 82*n + 12) / 12 with offset
 *            1 (a(1)=33)
 * Range:     u8 none, u16 n <= 6, u32 n <= 153, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;      // n^2
    seq_t n3 = n2 * n_seq;         // n^3
    seq_t n4 = n3 * n_seq;         // n^4
    seq_t num = (seq_t)11 * n4
              + (seq_t)98 * n3
              + (seq_t)193 * n2
              + (seq_t)82  * n_seq
              + (seq_t)12;
    return num / (seq_t)12;
}
