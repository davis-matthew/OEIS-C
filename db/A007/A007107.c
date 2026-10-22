/*
 * A007107  Number of labeled 2-regular digraphs with n nodes.
 * https://oeis.org/A007107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n*(n-1)*a(n-1) + (n+1)*n*(n-1)/2*a(n-2) +
 *            3*n*(n-1)*(n-2)/2*a(n-3) + n*(n-1)*(n-2)*(n-3)/2*a(n-4) + 1 -
 *            n^2, with a(0) = 1 and a(m) = 0 for m < 0 (derived from the EGF
 *            ODE (2x^2+x^3)A' - (2-3x^2-3x^3-x^4)A + 2(1-x-x^2)e^x = 0)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p1 = 0, p2 = 0, p3 = 0, p4 = 0; /* a(i-1), a(i-2), a(i-3), a(i-4) */
    seq_t cur = 1;                        /* a(0) */
    idx_t i;

    if (n == 0) return 1;
    p1 = 1;
    for (i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t c2 = (i >= 2) ? (seq_t)(i * (i - 1) / 2) : (seq_t)0; /* n(n-1)/2 */
        seq_t sum = 0;

        sum += si * (si - 1) * p1;
        if (i >= 2) {
            sum += c2 * (si + 1) * p2;
        }
        if (i >= 3) {
            sum += c2 * (si - 2) * 3 * p3;
        }
        if (i >= 4) {
            sum += c2 * (si - 2) * (si - 3) * p4;
        }
        sum += 1;
        sum -= si * si;

        cur = sum;
        p4 = p3;
        p3 = p2;
        p2 = p1;
        p1 = cur;
    }
    return cur;
}
