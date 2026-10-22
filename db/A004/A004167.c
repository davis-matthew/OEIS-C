/*
 * A004167  Powers of 3 written backwards.
 * https://oeis.org/A004167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse the digits of 3^n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power = 1;
    seq_t base = 3;
    idx_t exp = n;
    
    while (exp > 0) {
        if (exp & 1) {
            power *= base;
        }
        base *= base;
        exp >>= 1;
    }
    
    seq_t reversed = 0;
    while (power > 0) {
        reversed = reversed * 10 + (power % 10);
        power /= 10;
    }
    
    return reversed;
}
