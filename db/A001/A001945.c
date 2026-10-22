/*
 * A001945  a(n+6) = -a(n+5) + a(n+4) + 3a(n+3) + a(n+2) - a(n+1) - a(n). a(n)
 *          = sign(n) if abs(n)<=3.
 * https://oeis.org/A001945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n+6) = -a(n+5) + a(n+4) + 3a(n+3) + a(n+2) - a(n+1) - a(n),
 *            with a(n) = sign(n) for abs(n)<=3
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 78, u64 n <= 157,
 *            u128 n <= 315, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 1;
    
    seq_t a_n_minus_6 = (seq_t)(-1); // a(-2) = -1
    seq_t a_n_minus_5 = (seq_t)(-1); // a(-1) = -1
    seq_t a_n_minus_4 = 0;           // a(0)
    seq_t a_n_minus_3 = 1;           // a(1)
    seq_t a_n_minus_2 = 1;           // a(2)
    seq_t a_n_minus_1 = 1;           // a(3)
    
    seq_t result = 0;
    
    for (idx_t i = 4; i <= n; i++) {
        result = -a_n_minus_1 + a_n_minus_2 + 3 * a_n_minus_3 + a_n_minus_4 - a_n_minus_5 - a_n_minus_6;
        
        a_n_minus_6 = a_n_minus_5;
        a_n_minus_5 = a_n_minus_4;
        a_n_minus_4 = a_n_minus_3;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = result;
    }
    
    return result;
}
