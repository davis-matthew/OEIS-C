/*
 * A004722  Delete all digits 3 from the terms of the sequence of nonnegative
 *          integers.
 * https://oeis.org/A004722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n for n < 3; a(n) = A004178(n + offset), where offset
 *            depends on the range of n, and A004178(k) is computed by
 *            removing all 3's from k (or 0 if empty)
 * Range:     u8 n <= 253, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)n;
    
    // Determine offset based on which range n falls into
    idx_t boundary = 2;
    idx_t offset = 0;
    idx_t increment = 29;
    
    while (n > boundary) {
        boundary += increment;
        offset++;
        increment = increment * 10 + 9;
    }
    
    // Compute A004178(n + offset) by removing all 3's from n + offset
    idx_t m = n + offset;
    seq_t result = 0;
    seq_t power = 1;
    
    while (m > 0) {
        idx_t digit = m % 10;
        if (digit != 3) {
            result = result + (seq_t)digit * power;
            power *= 10;
        }
        m /= 10;
    }
    
    return result;
}
