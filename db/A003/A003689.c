/*
 * A003689  Number of Hamiltonian paths in K_3 X P_n.
 * https://oeis.org/A003689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128 * 3^(n-2) - (21*n + 57) * 2^(n-2), n>2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 3;
    if (n == 2) return 30;
    
    seq_t n_seq = (seq_t)n;
    idx_t exp = n - 2;
    
    seq_t pow3 = pow_seq(3, exp);
    seq_t pow2 = pow_seq(2, exp);
    
    seq_t term1 = 128 * pow3;
    seq_t term2 = (21 * n_seq + 57) * pow2;
    
    return term1 - term2;
}
