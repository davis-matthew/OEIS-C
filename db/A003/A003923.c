/*
 * A003923  Order of universal Chevalley group B_n (2) or symplectic group
 *          Sp(2n,2).
 * https://oeis.org/A003923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(n^2) * ∏(i=1 to n) (4^i - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, seq_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t nn = (seq_t)n;
    seq_t result = 1;
    
    seq_t n_squared = nn * nn;
    result = pow_seq((seq_t)2, n_squared);
    
    for (seq_t i = 1; i <= nn; i++) {
        seq_t power_of_4 = pow_seq((seq_t)4, i);
        result *= (power_of_4 - 1);
    }
    
    return result;
}
