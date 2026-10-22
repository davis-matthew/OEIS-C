/*
 * A006938  Convert the last term from decimal to binary! a(1)=3.
 * https://oeis.org/A006938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 3; a(n) = binary representation of a(n-1) interpreted as
 *            decimal
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    
    seq_t val = (seq_t)3;
    for (idx_t i = 2; i <= n; i++) {
        seq_t result = 0;
        seq_t power_of_10 = 1;
        seq_t x = val;
        
        while (x > 0) {
            seq_t bit = x % 2;
            result += bit * power_of_10;
            power_of_10 *= 10;
            x /= 2;
        }
        
        val = result;
    }
    
    return val;
}
