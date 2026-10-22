/*
 * A001813  Quadruple factorial numbers: a(n) = (2n)!/n!.
 * https://oeis.org/A001813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2*n)!/n! = Product_{k=1..n} (n+k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t n_seq = (seq_t)n;
    for (seq_t k = 1; k <= n_seq; k++) {
        result *= (n_seq + k);
    }
    return result;
}
