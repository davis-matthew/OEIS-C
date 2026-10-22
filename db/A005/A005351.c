/*
 * A005351  Base -2 representation for n regarded as base 2, then evaluated.
 * https://oeis.org/A005351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice, look
 * Formula:   Base -2 representation of n, interpreted as base 2
 * Range:     u8 n <= 85, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    int64_t m = (int64_t)n;
    
    while (m != 0) {
        if (m % 2 != 0) {
            result += power;
            m = -(m - 1) / 2;
        } else {
            m = -m / 2;
        }
        power *= 2;
    }
    
    return result;
}
