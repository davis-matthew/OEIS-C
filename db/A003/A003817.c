/*
 * A003817  a(0) = 0, a(n) = a(n-1) OR n.
 * https://oeis.org/A003817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   for n > 0, if 2^(k-1) <= n < 2^k then a(n) = 2^k - 1; a(0) = 0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    // Find the position of the highest set bit in n
    idx_t msb = 0;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        msb++;
    }
    
    // a(n) = 2^(msb + 1) - 1
    seq_t result = 1;
    for (idx_t i = 0; i < msb; i++) {
        result = result * 2 + 1;
    }
    return result;
}
