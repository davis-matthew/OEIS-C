/*
 * A004094  Powers of 2 written backwards.
 * https://oeis.org/A004094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A004086(A000079(n)), where A000079(n) = 2^n and A004086
 *            reverses digits
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute 2^n using binary exponentiation
    seq_t power_of_2 = 1;
    seq_t base = 2;
    idx_t exp = n;
    
    while (exp > 0) {
        if (exp & 1) {
            power_of_2 *= base;
        }
        base *= base;
        exp >>= 1;
    }
    
    // Reverse the digits
    seq_t reversed = 0;
    seq_t temp = power_of_2;
    while (temp > 0) {
        seq_t digit = temp % 10;
        reversed = reversed * 10 + digit;
        temp /= 10;
    }
    
    return reversed;
}
