/*
 * A000748  Expansion of bracket function.
 * https://oeis.org/A000748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, eigen
 * Formula:   a(6m+r) = c_r * (-27)^m with c = {1, -3, 6, -9, 9, 0}, which is
 *            the closed form of a(n) = -3*a(n-1) - 3*a(n-2), a(0) = 1, a(1) =
 *            -3
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 37, i64 n <= 78, i128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int c[6] = {1, -3, 6, -9, 9, 0};
    idx_t m = n / 6;
    int r = (int)(n % 6);
    seq_t p = 1;
    idx_t i;
    if (c[r] == 0) return 0;
    for (i = 0; i < m; i++) p *= -27;
    return p * c[r];
}
