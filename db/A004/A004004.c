/*
 * A004004  a(n) = (3^(2*n+1) - 8*n - 3)/16.
 * https://oeis.org/A004004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (3^(2*n-1) - 1)/2, a(0) = 0; with h(k) =
 *            (3^(2k-1)-1)/2, h(1) = 1, h(k+1) = 9*h(k) + 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s = 0;
    seq_t h = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        s += h;
        if (k < n) h = h * 9 + 4;
    }
    return s;
}
