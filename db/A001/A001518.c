/*
 * A001518  Bessel polynomial y_n(3).
 * https://oeis.org/A001518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3(2n-1)*a(n-1) + a(n-2), with a(0)=1, a(1)=4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 4;
    
    seq_t prev2 = 1;
    seq_t prev1 = 4;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t i_seq = (seq_t)i;
        seq_t coeff = 3 * (2 * i_seq - 1);
        seq_t curr = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
