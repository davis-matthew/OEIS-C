/*
 * A006996  a(n) = C(2n,n) mod 3.
 * https://oeis.org/A006996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n,n) mod 3 using Lucas' theorem in base 3
 * Range:     u8 n <= 2187, u16 n <= 2187, u32 n <= 2187, u64 n <= 2187,
 *            u128 n <= 2187, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t n2 = 2 * n;
    seq_t result = 1;
    
    while (n > 0 || n2 > 0) {
        idx_t digit_n = n % 3;
        idx_t digit_n2 = n2 % 3;
        
        if (digit_n > digit_n2) {
            return 0;
        }
        
        seq_t binom;
        if (digit_n == 0) {
            binom = 1;
        } else if (digit_n == 1) {
            binom = (digit_n2 == 1) ? 1 : 2;
        } else {
            binom = 1;
        }
        
        result = (result * binom) % 3;
        
        n /= 3;
        n2 /= 3;
    }
    
    return result;
}
