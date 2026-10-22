/*
 * A001872  Convolved Fibonacci numbers.
 * https://oeis.org/A001872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. 1/(1 - x - x^2)^4. With g_0(n) = [n=0] and g_k(n) =
 *            g_k(n-1) + g_k(n-2) + g_{k-1}(n), g_k(-1) = g_k(-2) = 0, we get
 *            a(n) = g_4(n)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 30, u64 n <= 71, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p1[5], p2[5], cur[5];
    int k;
    idx_t i;
    for (k = 0; k < 5; k++) { p1[k] = 0; p2[k] = 0; cur[k] = 0; }
    for (i = 0; i <= n; i++) {
        cur[0] = (i == 0) ? (seq_t)1 : (seq_t)0;
        for (k = 1; k < 5; k++) {
            cur[k] = p1[k] + p2[k] + cur[k - 1];
        }
        for (k = 1; k < 5; k++) {
            p2[k] = p1[k];
            p1[k] = cur[k];
        }
    }
    return cur[4];
}
