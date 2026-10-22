/*
 * A004182  Omit 7's from n.
 * https://oeis.org/A004182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   omit all 7's from the decimal digits of n
 * Range:     u8 n <= 255, u16 n <= 6529, u32 n <= 6529, u64 n <= 6529,
 *            u128 n <= 6529, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
        
    if (n < 7) return (seq_t)n;
    
    seq_t result = 0;
    seq_t power = 1;
    idx_t temp = n;
    
    while (temp > 0) {
        idx_t digit = temp % 10;
        if (digit != 7) {
            result = result + ((seq_t)digit) * power;
            power = power * 10;
        }
        temp = temp / 10;
    }
    
    return result;
}
