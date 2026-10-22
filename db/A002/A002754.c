/*
 * A002754  Related to coefficient of m in Jacobi elliptic function cn(z, m).
 * https://oeis.org/A002754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 4*(n-1), a(0) = 0, a(1) = 0 (equivalently a(n)
 *            = (9^n-8*n-1)/16)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 0;
    idx_t k;
    for (k = 2; k <= n; k++) {
        r = r * 9 + (seq_t)(k - 1) * 4;
    }
    return r;
}
