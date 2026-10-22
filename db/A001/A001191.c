/*
 * A001191  Digits of positive squares.
 * https://oeis.org/A001191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1375, u32 n <= 5500, u64 n <= 5500,
 *            u128 n <= 5500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digits of positive squares: a(n) is the n-th digit in the concatenated sequence 1^2, 2^2, 3^2, ... */

seq_t a(idx_t n) {
    idx_t digit_count = 0;
    idx_t k = 1;
    
    while (1) {
        seq_t square = (seq_t)k * (seq_t)k;
        
        // Extract and count digits
        seq_t temp = square;
        int digits[200];  // Buffer for digits (in reverse order)
        int num_digits = 0;
        
        do {
            digits[num_digits++] = (int)(temp % 10);
            temp /= 10;
        } while (temp > 0);
        
        // Check if n falls within this square's digits
        if (digit_count + num_digits >= n) {
            // n-th digit is in this square
            idx_t pos_from_left = n - digit_count - 1;  // 0-indexed from left
            return (seq_t)digits[num_digits - 1 - pos_from_left];
        }
        
        digit_count += num_digits;
        k++;
    }
}
