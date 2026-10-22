/*
 * A007932  Numbers that contain only 1's, 2's and 3's.
 * https://oeis.org/A007932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   bijective base-3 representation interpreted as decimal
 * Range:     u8 n <= 30, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    idx_t temp = n;
    
    while (temp > 0) {
        seq_t digit = (seq_t)((temp - 1) % 3) + 1;
        result = result + digit * power;
        power = power * 10;
        temp = (temp - 1) / 3;
    }
    
    return result;
}
