/*
 * A004720  Delete all digits '1' from the sequence of nonnegative integers.
 * https://oeis.org/A004720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice, look
 * Formula:   Remove all digits '1' from each nonnegative integer; a(n) is the
 *            n-th integer that produces a non-empty result
 * Range:     u8 n <= 253, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    
    while (1) {
        seq_t value = 0;
        int has_digit = 0;
        
        if (k == 0) {
            value = 0;
            has_digit = 1;
        } else {
            idx_t temp = k;
            seq_t power = 1;
            
            while (temp > 0) {
                int digit = temp % 10;
                if (digit != 1) {
                    value = value + (seq_t)digit * power;
                    power = power * 10;
                    has_digit = 1;
                }
                temp = temp / 10;
            }
        }
        
        if (has_digit) {
            count++;
            if (count == n) {
                return value;
            }
        }
        
        k++;
    }
}
