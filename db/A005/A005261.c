/*
 * A005261  a(n) = Sum_{k = 0..n} C(n,k)^5.
 * https://oeis.org/A005261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} C(n,k)^5, with C(n,k+1) =
 *            C(n,k)*(n-k)/(k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t c = 1;
    for (idx_t k = 0; k <= n; k++) {
        seq_t c2 = c * c;
        sum += c2 * c2 * c;
        if (k < n)
            c = c * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return sum;
}
