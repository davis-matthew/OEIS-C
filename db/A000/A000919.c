/*
 * A000919  a(n) = 4^n - C(4,3)*3^n + C(4,2)*2^n - C(4,1).
 * https://oeis.org/A000919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 4*3^n + 6*2^n - 4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow4 = (seq_t)1;
    seq_t pow3 = (seq_t)1;
    seq_t pow2 = (seq_t)1;
    
    for (idx_t i = 0; i < n; i++) {
        pow4 *= (seq_t)4;
        pow3 *= (seq_t)3;
        pow2 *= (seq_t)2;
    }
    
    return pow4 - (seq_t)4 * pow3 + (seq_t)6 * pow2 - (seq_t)4;
}
