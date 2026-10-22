/*
 * A004053  For m=2,3,..., write m in bases 2,3,..,m.
 * https://oeis.org/A004053
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   Row m contains m in bases 2,3,...,m. Read triangle row by row
 * Range:     u8 n <= 22, u16 n <= 191, u32 n <= 191, u64 n <= 191,
 *            u128 n <= 191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Find which row m (row m has m-1 entries and m(m-1)/2 + 1 is its last index)
    idx_t m = 2;
    while (m * (m - 1) / 2 + 1 < n) {
        m++;
    }
    
    // Starting index of row m
    idx_t row_start = 2 + (m - 2) * (m - 1) / 2;
    
    // Position within row m (0-indexed) and corresponding base
    idx_t pos = n - row_start;
    idx_t base = 2 + pos;
    
    // Convert m to given base and interpret digits as decimal
    idx_t digits[64];
    idx_t num_digits = 0;
    idx_t temp = (idx_t)m;
    
    while (temp > 0) {
        digits[num_digits++] = temp % base;
        temp /= base;
    }
    
    // Build result from digits (currently in reverse order)
    seq_t result = 0;
    for (idx_t i = num_digits; i > 0; i--) {
        result = result * 10 + (seq_t)digits[i - 1];
    }
    
    return result;
}
