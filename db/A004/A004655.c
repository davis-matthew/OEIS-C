/*
 * A004655  Powers of 2 written in base 16.
 * https://oeis.org/A004655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^(n mod 4) * 10^floor(n/4)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 38, u64 n <= 76, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow2 = (seq_t)1 << (n % 4);
    
    seq_t pow10 = 1;
    idx_t exp = n / 4;
    for (idx_t i = 0; i < exp; i++) {
        pow10 *= 10;
    }
    
    return pow2 * pow10;
}
