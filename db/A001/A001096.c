/*
 * A001096  a(n) = n + n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5).
 * https://oeis.org/A001096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5) for n >= 6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 6) {
        return (seq_t)n;
    }
    
    seq_t sn = (seq_t)n;
    seq_t prod = sn * (sn - 1) * (sn - 2) * (sn - 3) * (sn - 4) * (sn - 5);
    
    return sn + prod;
}
