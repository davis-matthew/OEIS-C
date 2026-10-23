/*
 * A036499  Numbers of the form k*(k+1)/6 for k = 2 or 3 modulo 6.
 * https://oeis.org/A036499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k+1) = (2k+1)*(3k+1) a(2k+2) = (2k+1)*(3k+2) for k >= 0 (so
 *            a(1)=1, a(2)=2, …)
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n ≥ 1 */
    if (n & 1) { /* odd index: n = 2k+1 */
        seq_t k = (seq_t)(n - 1) / 2;
        return (2 * k + 1) * (3 * k + 1);
    } else {    /* even index: n = 2k+2, k = n/2 - 1 */
        seq_t k = (seq_t)(n / 2 - 1);
        /* equivalently, m = n/2,  a(n) = (2*m-1)*(3*m-1) */
        seq_t m = (seq_t)(n / 2);
        return (2 * m - 1) * (3 * m - 1);
    }
}
