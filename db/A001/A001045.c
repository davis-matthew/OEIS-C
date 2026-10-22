/*
 * A001045  Jacobsthal sequence (or Jacobsthal numbers): a(n) = a(n-1) +
 *          2*a(n-2), with a(0) = 0, a(1) = 1; also a(n) = nearest integer to
 *          2^n/3.
 * https://oeis.org/A001045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, core
 * Formula:   a(n) = 2*a(n-1) - (-1)^n, a(0) = 0 (i.e. a(2k+1) = 2*a(2k)+1,
 *            a(2k) = 2*a(2k-1)-1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)0;
    idx_t i;
    for (i = 1; i <= n; i++) {
        x = x * 2;
        if (i & 1) {
            x += 1;
        } else {
            x -= 1;
        }
    }
    return x;
}
