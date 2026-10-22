/*
 * A005920  Tricapped prism numbers.
 * https://oeis.org/A005920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n+1)*(3*n^2+4*n+2)/2
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 1419, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t n_plus_1 = sn + 1;
    seq_t poly = 3 * sn * sn + 4 * sn + 2;
    
    if (n_plus_1 % 2 == 0) {
        return (n_plus_1 / 2) * poly;
    } else {
        return n_plus_1 * (poly / 2);
    }
}
