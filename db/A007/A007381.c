/*
 * A007381  7th-order maximal independent sets in path graph.
 * https://oeis.org/A007381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n+9), where b(m) = b(m-2) + b(m-9), b(0) = 1, b(m) = 0
 *            for m < 0 (b(m) = T(2,9,m), the number of compositions of m into
 *            parts 2 and 9)
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ring[10];
    idx_t m, target = n + 9;
    int i;
    for (i = 0; i < 10; i++) ring[i] = 0;
    for (m = 0; m <= target; m++) {
        seq_t v = 0;
        if (m == 0) v = 1;
        if (m >= 2) v += ring[(m - 2) % 10];
        if (m >= 9) v += ring[(m - 9) % 10];
        ring[m % 10] = v;
    }
    return ring[target % 10];
}
