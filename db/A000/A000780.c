/*
 * A000780  a(n) = (n+1)!/2 + (n-1)(n-1)!.
 * https://oeis.org/A000780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * (n^2+3*n-2)/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t factorial = 1;
    for (idx_t i = 2; i < n; i++) {
        factorial *= (seq_t)i;
    }
    seq_t n_seq = (seq_t)n;
    seq_t coeff = (n_seq * n_seq + 3 * n_seq - 2) / 2;
    return factorial * coeff;
}
