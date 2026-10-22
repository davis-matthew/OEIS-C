/*
 * A001465  Number of degree-n odd permutations of order 2.
 * https://oeis.org/A001465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + (n-1)*(n-3)*a(n-4) -
 *            (n-3)*(n-4)*a(n-5)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 32, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 3;
    if (n == 4) return 6;
    
    seq_t a_vals[5];
    a_vals[0] = 0;
    a_vals[1] = 0;
    a_vals[2] = 1;
    a_vals[3] = 3;
    a_vals[4] = 6;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t nn = (seq_t)i;
        seq_t new_val = 3 * a_vals[4] - 3 * a_vals[3] + a_vals[2] 
                      + (nn - 1) * (nn - 3) * a_vals[1] 
                      - (nn - 3) * (nn - 4) * a_vals[0];
        
        a_vals[0] = a_vals[1];
        a_vals[1] = a_vals[2];
        a_vals[2] = a_vals[3];
        a_vals[3] = a_vals[4];
        a_vals[4] = new_val;
    }
    
    return a_vals[4];
}
