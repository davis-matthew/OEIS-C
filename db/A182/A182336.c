/*
 * A182336  a(n) is the least m>=n, such that the Hamming distance D(n,m)=4.
 * https://oeis.org/A182336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least m ≥ n such that the binary Hamming distance between
 *            n and m is 4
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    for (;;) {
        seq_t diff = (seq_t)n ^ m;
        unsigned cnt = 0;
        while (diff) {
            diff &= diff - 1;  /* remove lowest set bit */
            ++cnt;
            if (cnt > 4) break; /* early exit if already too many differences */
        }
        if (cnt == 4) return m;
        ++m;
    }
}
