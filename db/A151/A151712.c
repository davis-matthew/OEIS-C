/*
 * A151712  a(n) = A048883(n) + 1.
 * https://oeis.org/A151712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) = 3^popcount(n) + 1
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned int popcount_u64(idx_t n) {
    unsigned int cnt = 0;
    while (n) {
        n &= n - 1;   /* clears the least significant set bit */
        cnt++;
    }
    return cnt;
}

seq_t a(idx_t n) {
    unsigned int p = popcount_u64(n);
    seq_t res = 1;
    while (p--) {
        res *= 3;
    }
    return res + 1;
}
