/*
 * A007376  The almost-natural numbers: write n in base 10 and juxtapose
 *          digits.
 * https://oeis.org/A007376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nice, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9841, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digit at position n in the concatenation of 0, 1, 2, ..., 9, 10, 11, ..., 99, 100, ... */
seq_t a(idx_t n) {
    if (n < 10) {
        return (seq_t)n;
    }
    
    idx_t pos = 10;
    idx_t digits = 2;
    idx_t start = 10;
    
    while (1) {
        idx_t count = 9;
        for (idx_t i = 1; i < digits; i++) {
            count *= 10;
        }
        idx_t total_digits = count * digits;
        
        if (pos + total_digits > n) {
            idx_t offset = n - pos;
            idx_t number_index = offset / digits;
            idx_t digit_index = offset % digits;
            idx_t k = start + number_index;
            
            idx_t divisor = 1;
            for (idx_t i = 0; i < digits - digit_index - 1; i++) {
                divisor *= 10;
            }
            return (seq_t)((k / divisor) % 10);
        }
        
        pos += total_digits;
        digits++;
        start *= 10;
    }
}
