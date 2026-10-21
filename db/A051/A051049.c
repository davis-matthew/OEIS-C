/*
 * A051049  Number of moves needed to solve an (n+1)-ring baguenaudier if two
 *          simultaneous moves of the two end rings are counted as one.
 * https://oeis.org/A051049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - (n mod 2) (i.e. 2^n for even n, 2^n-1 for odd n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
    }
    pow2 -= (seq_t)(n & 1);
    return pow2;
}
