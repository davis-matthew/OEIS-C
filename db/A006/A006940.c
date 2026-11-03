/*
 * A006940  Rows of Pascal's triangle mod 3.
 * https://oeis.org/A006940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   rows of Pascal's triangle mod 3, interpreted as decimal numbers
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int binom_mod_3(idx_t n, idx_t k) {
    if (k > n) return 0;
    
    int result = 1;
    while (n > 0 || k > 0) {
        int n_digit = n % 3;
        int k_digit = k % 3;
        
        if (k_digit > n_digit) return 0;
        
        int c_val;
        if (k_digit == 0) {
            c_val = 1;
        } else if (k_digit == 1) {
            c_val = n_digit;
        } else {
            c_val = (n_digit == 2) ? 1 : 0;
        }
        
        result = (result * c_val) % 3;
        n /= 3;
        k /= 3;
    }
    
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t k = 0; k <= n; k++) {
        int digit = binom_mod_3(n, k);
        result = result * (seq_t)10 + (seq_t)digit;
    }
    
    return result;
}
