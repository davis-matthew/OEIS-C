/*
 * A004178  Omit 3's from n.
 * https://oeis.org/A004178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Remove all 3's from the decimal representation of n */
seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t multiplier = 1;
    idx_t temp = n;
    
    while (temp > 0) {
        idx_t digit = temp % 10;
        if (digit != 3) {
            result = result + (seq_t)digit * multiplier;
            multiplier *= 10;
        }
        temp /= 10;
    }
    
    return result;
}
