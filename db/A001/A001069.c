/*
 * A001069  Log2*(n) (version 2): take log_2 of n this many times to get a
 *          number < 2.
 * https://oeis.org/A001069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of times we must apply floor(log_2) to n to get <
 *            2
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t count = 0;
    
    while (x >= 2) {
        // Compute floor(log_2(x)) by counting right shifts until x <= 1
        seq_t log2_x = 0;
        seq_t temp = x;
        while (temp > 1) {
            temp >>= 1;
            log2_x++;
        }
        x = log2_x;
        count++;
    }
    
    return count;
}
