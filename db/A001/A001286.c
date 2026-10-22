/*
 * A001286  Lah numbers: a(n) = (n-1)*n!/2.
 * https://oeis.org/A001286
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*n!/2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    
    // Compute (n-1)!
    for (idx_t i = 2; i < n; i++) {
        result *= i;
    }
    
    // a(n) = (n-1)! * (n-1) * n / 2
    seq_t n_minus_1 = (seq_t)(n - 1);
    seq_t n_seq = (seq_t)n;
    
    if (n_seq % 2 == 0) {
        result *= n_minus_1;
        result *= (n_seq / 2);
    } else {
        result *= (n_minus_1 / 2);
        result *= n_seq;
    }
    
    return result;
}
