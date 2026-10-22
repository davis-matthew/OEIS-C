/*
 * A000478  Number of ways of placing n labeled balls into 3 indistinguishable
 *          boxes with at least 2 balls in each box.
 * https://oeis.org/A000478
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*a(n-1) + (2^(n-3) - n + 1)*(n-1), a(n) = 0 for n < 6
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 0;
    seq_t p = 8; /* 2^(k-3) for k = 6 */
    idx_t k;
    for (k = 6; k <= n; k++) {
        r = r * 3 + (p - (seq_t)k + 1) * ((seq_t)k - 1);
        p = p * 2;
    }
    return r;
}
