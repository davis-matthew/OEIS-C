/*
 * A004286  Least positive multiple of n written in base 6 using only 0 and 1.
 * https://oeis.org/A004286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 69, u64 n <= 1504,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least positive multiple of n whose base-6 representation uses only 0 and 1 */

seq_t a(idx_t n) {
    for (idx_t k = 1; ; k++) {
        seq_t decimal_value = 0;
        seq_t power_of_6 = 1;
        idx_t temp_k = k;
        while (temp_k > 0) {
            if (temp_k & 1) {
                decimal_value += power_of_6;
            }
            power_of_6 *= 6;
            temp_k >>= 1;
        }
        
        if (decimal_value % (seq_t)n == 0) {
            idx_t num_bits = 0;
            idx_t temp_k2 = k;
            while (temp_k2 > 0) {
                num_bits++;
                temp_k2 >>= 1;
            }
            
            seq_t result = 0;
            for (idx_t i = num_bits; i > 0; i--) {
                result = result * 10;
                if ((k >> (i - 1)) & 1) {
                    result += 1;
                }
            }
            
            return result;
        }
    }
}
