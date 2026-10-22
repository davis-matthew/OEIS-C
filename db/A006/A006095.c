/*
 * A006095  Gaussian binomial coefficient [n, 2] for q = 2.
 * https://oeis.org/A006095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*a(n-1) + 2^(n-1) - 1 for n >= 2, a(0) = 0, a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 0;
    seq_t p = 1;
    idx_t k;
    if (n < 2) return 0;
    for (k = 2; k <= n; k++) {
        p += p;
        r = r * 4 + p - 1;
    }
    return r;
}
