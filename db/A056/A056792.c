/*
 * A056792  Minimal number of steps to get from 0 to n by (a) adding 1 or (b)
 *          multiplying by 2.
 * https://oeis.org/A056792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = popcount(n) + floor(log2(n)) for n>0 (a(0)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t pop = 0;
    idx_t val = n;
    idx_t log2 = 0;
    while (val > 1) {
        if (val & 1) ++pop;
        val >>= 1;
        ++log2;
    }
    if (val & 1) ++pop;
    return pop + (seq_t)log2;
}
