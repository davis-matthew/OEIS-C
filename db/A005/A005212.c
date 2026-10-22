/*
 * A005212  a(n) = n! if n is odd otherwise 0 (from the Taylor series for sin
 *          x).
 * https://oeis.org/A005212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! if n is odd, otherwise 0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n % 2 == 0) {
        return 0;
    }
    
    seq_t result = 1;
    for (seq_t i = 2; i <= (seq_t)n; i++) {
        result *= i;
    }
    return result;
}
