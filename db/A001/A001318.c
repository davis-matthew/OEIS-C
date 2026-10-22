/*
 * A001318  Generalized pentagonal numbers: m*(3*m - 1)/2, m = 0, +-1, +-2,
 *          +-3, ....
 * https://oeis.org/A001318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = m*(3*m - 1)/2 for m = 0, 1, -1, 2, -2, 3, -3, ...
 *            (sequence indexed by n)
 * Range:     u8 n <= 25, u16 n <= 417, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    
    seq_t k;
    if (n & 1) {
        /* n is odd: n = 2k+1, m = k+1 */
        k = (seq_t)((n + 1) / 2);
        return k * (3 * k - 1) / 2;
    } else {
        /* n is even: n = 2k, m = -k, result = k*(3*k + 1)/2 */
        k = (seq_t)(n / 2);
        return k * (3 * k + 1) / 2;
    }
}
