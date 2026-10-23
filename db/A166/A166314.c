/*
 * A166314  Number of 1's in binary expansion of A000124(n).
 * https://oeis.org/A166314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = popcount(n*(n+1)/2 + 1)
 * Range:     u8 n <= 22, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* count number of 1-bits in the binary representation of a seq_t value */
static seq_t popcount_seq(seq_t x) {
    seq_t cnt = 0;
    while (x) {
        cnt += x & (seq_t)1;
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn + 1) / 2 + 1;
    return popcount_seq(val);
}
