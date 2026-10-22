/*
 * A006640  Restricted postage stamp problem.
 * https://oeis.org/A006640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(n+1) if n is odd, 2*n^2 + 4*n - 8 if n is even
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t result = 2 * n_seq * (n_seq + 1);
    if (n % 2 == 0) {
        result = result + 2 * n_seq - 8;
    }
    return result;
}
