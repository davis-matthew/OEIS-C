/*
 * A002453  Central factorial numbers: 2nd subdiagonal of A008958.
 * https://oeis.org/A002453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) + A002452(n+1), a(0) = 1, where A002452(n+1) =
 *            9*A002452(n) + 1, A002452(1) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s = 1;   /* A002452(i+1) */
    seq_t r = 1;   /* a(i) */
    idx_t i;
    for (i = 1; i <= n; i++) {
        s = s * 9 + 1;
        r = r * 25 + s;
    }
    return r;
}
