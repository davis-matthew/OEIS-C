/*
 * A023855  a(n) = 1*(n) + 2*(n-1) + 3*(n-2) + ... + (n+1-k)*k, where k =
 *          floor((n+1)/2).
 * https://oeis.org/A023855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n is odd) ? (n+1)*(n+3)*(2*n+1)/24 : n*(n+1)*(n+2)/12
 * Range:     u8 n <= 13, u16 n <= 90, u32 n <= 2145, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if (n & 1) {
        seq_t t1 = sn + 1;
        seq_t t2 = sn + 3;
        seq_t t3 = 2 * sn + 1;
        return t1 * t2 * t3 / 24;
    } else {
        seq_t t1 = sn;
        seq_t t2 = sn + 1;
        seq_t t3 = sn + 2;
        return t1 * t2 * t3 / 12;
    }
}
