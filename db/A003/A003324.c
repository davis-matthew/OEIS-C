/*
 * A003324  A nonrepetitive sequence.
 * https://oeis.org/A003324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n mod 4 for odd n; for even n, write n = (2*k+1) * 2^e,
 *            then a(n) = 2 if k+e is odd, 4 if k+e is even
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n & 1) {
        return (seq_t)(n & 3);
    }
    
    idx_t m = n;
    idx_t e = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        e++;
    }
    
    idx_t k = (m - 1) >> 1;
    
    return ((k + e) & 1) ? 2 : 4;
}
