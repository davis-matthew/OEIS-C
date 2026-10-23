/*
 * A147666  List of triples (0, 6n+1, 6n+5) for n = 0, 1, 2, ...
 * https://oeis.org/A147666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 0 if n%3==0; 6*(n/3)+1 if n%3==1; 6*(n/3)+5 if n%3==2
 * Range:     u8 n <= 127, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t k = (seq_t)(n / 3);
    idx_t r = n % 3;
    if (r == 0) {
        return 0;
    } else if (r == 1) {
        return k * 6 + 1;
    } else {
        return k * 6 + 5;
    }
}
