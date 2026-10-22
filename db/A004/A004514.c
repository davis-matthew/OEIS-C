/*
 * A004514  Generalized nim sum n + n in base 4.
 * https://oeis.org/A004514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   In the base 4 expansion of 2*n + 1, change 1 to 0 and 3 to 2
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n * 2 + 1;
    seq_t result = 0;
    seq_t power = 1;
    
    while (m > 0) {
        seq_t digit = m % 4;
        if (digit == 1) {
            digit = 0;
        } else if (digit == 3) {
            digit = 2;
        }
        result += digit * power;
        power *= 4;
        m /= 4;
    }
    
    return result;
}
