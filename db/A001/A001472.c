/*
 * A001472  Number of degree-n permutations of order dividing 4.
 * https://oeis.org/A001472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1) + (n-1)*a(n-2) + (n-1)*(n-2)*(n-3)*a(n-4), a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w0 = 1, w1 = 1, w2 = 2, w3 = 4; /* a(i-4), a(i-3), a(i-2), a(i-1) */
    idx_t i;
    if (n == 0) return w0;
    if (n == 1) return w1;
    if (n == 2) return w2;
    if (n == 3) return w3;
    for (i = 4; i <= n; i++) {
        seq_t c = (seq_t)(i - 1) * (seq_t)(i - 2) * (seq_t)(i - 3);
        seq_t next = w3 + (seq_t)(i - 1) * w2 + c * w0;
        w0 = w1;
        w1 = w2;
        w2 = w3;
        w3 = next;
    }
    return w3;
}
