/*
 * A003691  Number of spanning trees with degrees 1 and 3 in K_3 X P_2n.
 * https://oeis.org/A003691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 8*a(n-2), a(1) = 3, a(2) = 36, a(3) = 324
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)36;
    if (n == 3) return (seq_t)324;
    
    seq_t prev2 = (seq_t)36;
    seq_t prev1 = (seq_t)324;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t curr = (seq_t)8 * prev1 + (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
