/*
 * A002457  a(n) = (2n+1)!/n!^2.
 * https://oeis.org/A002457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = (2n+1)!/n!^2, computed via a(n) = a(n-1)*2*(2n+1)/n, a(0)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        idx_t m = 2 * (2 * k + 1);
        idx_t d = k;
        if ((d & 1) == 0) {
            d /= 2;
            m /= 2;
        }
        /* gcd(m, d) = 1 here, so d divides r exactly */
        r = (r / (seq_t)d) * (seq_t)m;
    }
    return r;
}
