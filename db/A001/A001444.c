/*
 * A001444  Bending a piece of wire of length n+1 (configurations that can
 *          only be brought into coincidence by turning the figure over are
 *          counted as different).
 * https://oeis.org/A001444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (3^n + 3^floor(n/2))/2, computed as h(n) + h(floor(n/2))
 *            + 1 where h(k) = (3^k - 1)/2, h(0) = 0, h(k) = 3*h(k-1) + 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n / 2;
    idx_t k;
    seq_t h = 0;
    seq_t hm = 0;
    for (k = 1; k <= n; k++) {
        h = h * 3 + 1;
        if (k == m) hm = h;
    }
    return h + hm + 1;
}
