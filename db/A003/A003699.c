/*
 * A003699  Number of Hamiltonian cycles in C_4 X P_n.
 * https://oeis.org/A003699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(1) = 1, a(2) = 6, a(3) = 22
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)22;
    
    seq_t prev2 = (seq_t)6;
    seq_t prev1 = (seq_t)22;
    seq_t curr;
    
    for (idx_t i = 4; i <= n; i++) {
        curr = (seq_t)4 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
