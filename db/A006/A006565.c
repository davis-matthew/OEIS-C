/*
 * A006565  Number of ways to color vertices of a hexagon using <= n colors,
 *          allowing only rotations.
 * https://oeis.org/A006565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+n+1)*(n^2-2*n+2)/6
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 54, u64 n <= 2191,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t t = (n % 2 == 0) ? (nn / 2) * (nn + 1) : nn * ((nn + 1) / 2);
    seq_t q = nn * nn + nn + 1;
    if (n % 3 == 1) {
        q /= 3;
    } else {
        t /= 3;
    }
    seq_t r = (nn - 1) * (nn - 1) + 1;
    return t * q * r;
}
