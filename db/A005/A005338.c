/*
 * A005338  Number of ways in which n identical balls can be distributed among
 *          5 boxes in a row such that each pair of adjacent boxes contains at
 *          least 4 balls.
 * https://oeis.org/A005338
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 10*n^3 - 445*n^2 + 2690*n - 1656)/24 for n > 9;
 *            a(8) = 1, a(9) = 8
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 418, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 8) return 1;
    if (n == 9) return 8;
    
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t n4 = n3 * n_seq;
    
    seq_t result = n4 + 10 * n3 - 445 * n2 + 2690 * n_seq - 1656;
    return result / 24;
}
