/*
 * A007773  For any circular arrangement of 0..n-1, let S = sum of squares of
 *          every sum of two contiguous numbers; then a(n) = # of distinct
 *          values of S.
 * https://oeis.org/A007773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3-16*n+27)/6 for odd n >= 7, (n^3-16*n+30)/6 for even
 *            n >= 7; initial values a(1..6) = 1, 1, 1, 3, 8, 21. Uses
 *            n^3-16*n = (n-4)*n*(n+4) and divides out factors first so that
 *            intermediate values never exceed a(n)
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 2953, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const unsigned char small[7] = {0, 1, 1, 1, 3, 8, 21};
    seq_t f[3];
    int i;
    if (n < 7) return (seq_t)small[n];
    f[0] = (seq_t)(n - 4);
    f[1] = (seq_t)n;
    f[2] = (seq_t)(n + 4);
    for (i = 0; i < 3; i++) {
        if (f[i] % 3 == 0) { f[i] /= 3; break; }
    }
    if (n % 2 == 0) {
        for (i = 0; i < 3; i++) {
            if (f[i] % 2 == 0) { f[i] /= 2; break; }
        }
        return f[0] * f[1] * f[2] + 5;
    } else {
        /* (x*y*z - 3*... ) handled as (xyz - 1)/2 + 5 with x,y,z odd */
        seq_t x = f[0], y = f[1], z = f[2];
        return ((x - 1) / 2) * y * z + ((y - 1) / 2) * z + (z - 1) / 2 + 5;
    }
}
