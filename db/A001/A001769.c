/*
 * A001769  Expansion of 1/((1+x)*(1-x)^7).
 * https://oeis.org/A001769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+6,6) - a(n-1), a(0) = 1 (i.e. a(n) = Sum_{k=0..n}
 *            (-1)^(n-k)*C(k+6,6))
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 131, u64 n <= 5458,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t b[7];
    seq_t r;
    idx_t k;
    int j;
    for (j = 0; j < 7; j++) b[j] = 1;   /* b[j] = C(k+j, j) for k = 0 */
    r = 1;
    for (k = 1; k <= n; k++) {
        for (j = 1; j < 7; j++) b[j] += b[j - 1];
        r = b[6] - r;
    }
    return r;
}
