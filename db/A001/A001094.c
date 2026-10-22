/*
 * A001094  a(n) = n + n*(n-1)*(n-2)*(n-3).
 * https://oeis.org/A001094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + n*(n-1)*(n-2)*(n-3)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if (n < 4) {
        return sn;
    }
    seq_t prod = sn * (sn - 1) * (sn - 2) * (sn - 3);
    return sn + prod;
}
