/*
 * A223950  Number of 3 X n 0..1 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A223950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=8; for n>1, a(n) = (4*n^3 + 9*n^2 + 41*n)/6 + 2
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        /* n==1 gives the first term 8; n==0 returns 0 (undefined offset) */
        return (seq_t)(n == 1 ? 8 : 0);
    }
    unsigned long long nn = (unsigned long long)n;
    unsigned long long num = 4ULL * nn * nn * nn
                          + 9ULL * nn * nn
                          + 41ULL * nn;
    unsigned long long res = num / 6ULL + 2ULL;
    return (seq_t)res;
}
