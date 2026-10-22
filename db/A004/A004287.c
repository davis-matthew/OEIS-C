/*
 * A004287  Least positive multiple of n written in base 7 using only 0 and 1.
 * https://oeis.org/A004287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Find least positive multiple of n with base-7 representation
 *            using only 0 and 1
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
        
    for (idx_t i = 1; ; i++) {
        seq_t base10_value = 0;
        seq_t power_of_7 = 1;
        idx_t temp = i;
        
        while (temp > 0) {
            if (temp & 1) {
                base10_value += power_of_7;
            }
            power_of_7 *= 7;
            temp >>= 1;
        }
        
        if (base10_value % (seq_t)n == 0) {
            seq_t base7_repr = 0;
            seq_t power_of_10 = 1;
            seq_t temp_val = base10_value;
            
            while (temp_val > 0) {
                seq_t digit = temp_val % 7;
                base7_repr += digit * power_of_10;
                power_of_10 *= 10;
                temp_val /= 7;
            }
            
            return base7_repr;
        }
    }
}
