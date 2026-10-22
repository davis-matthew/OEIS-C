/*
 * A005359  a(n) = n! if n is even, otherwise 0 (from Taylor series for cos
 *          x).
 * https://oeis.org/A005359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! if n is even, otherwise 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n & 1) return 0;
    
    seq_t result = 1;
    for (idx_t i = 2; i <= n; i++) {
        result *= (seq_t)i;
    }
    return result;
}
