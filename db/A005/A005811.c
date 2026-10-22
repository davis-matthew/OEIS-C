/*
 * A005811  Number of runs in binary expansion of n (n>0); number of 1's in
 *          Gray code for n.
 * https://oeis.org/A005811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, core, nice, hear
 * Formula:   a(n) = number of 1 bits in Gray code of n; a(0) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    idx_t gray = n ^ (n >> 1);
    
    seq_t count = 0;
    while (gray > 0) {
        count += gray & 1;
        gray >>= 1;
    }
    return count;
}
