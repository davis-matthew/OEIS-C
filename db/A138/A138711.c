/*
 * A138711  n-th run has length n-th positive triangular number, with digits 0
 *          and 1 only, starting with 1.
 * https://oeis.org/A138711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (r mod 2), where r is the smallest integer such that
 *            r(r+1)(r+2)/6 >= n
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = 0;
    seq_t n_seq = (seq_t)n;
    while (1) {
        seq_t s = ((seq_t)r * (r + 1) * (r + 2)) / 6;
        if (s >= n_seq) break;
        r++;
    }
    return (seq_t)(r & 1);
}
