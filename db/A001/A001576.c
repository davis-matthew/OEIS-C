/*
 * A001576  a(n) = 1^n + 2^n + 4^n.
 * https://oeis.org/A001576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 2^n + 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow2 = 1;
    seq_t base = 2;
    idx_t exp = n;
    
    while (exp > 0) {
        if (exp & 1) pow2 *= base;
        base *= base;
        exp >>= 1;
    }
    
    seq_t pow4 = pow2 * pow2;
    
    return 1 + pow2 + pow4;
}
