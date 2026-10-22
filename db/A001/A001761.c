/*
 * A001761  a(n) = (2*n)!/(n+1)!.
 * https://oeis.org/A001761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)!/(n+1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t result = 1;
    for (seq_t k = n_seq + 2; k <= 2 * n_seq; k++) {
        result *= k;
    }
    return result;
}
