/*
 * A003230  Expansion of 1/((1-x)*(1-2*x)*(1-x-2*x^3)).
 * https://oeis.org/A003230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = a(n+2) + 2*a(n) + 2^(n+4) - 1, with a(0)=1, a(1)=4,
 *            a(2)=11
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)11;
    
    seq_t a_n_minus_3 = (seq_t)1;   /* a(0) */
    seq_t a_n_minus_2 = (seq_t)4;   /* a(1) */
    seq_t a_n_minus_1 = (seq_t)11;  /* a(2) */
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t power_of_2 = (seq_t)1 << (i + 1);
        seq_t a_i = a_n_minus_1 + ((seq_t)2 * a_n_minus_3) + power_of_2 - (seq_t)1;
        
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_i;
    }
    
    return a_n_minus_1;
}
