/*
 * A002119  Bessel polynomial y_n(-2).
 * https://oeis.org/A002119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -2(2n-1)*a(n-1) + a(n-2), a(0) = 1, a(1) = -1 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return -1;
    
    seq_t prev2 = 1;
    seq_t prev1 = -1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t i_seq = (seq_t)i;
        seq_t coeff = -2 * (2 * i_seq - 1);
        seq_t curr = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
