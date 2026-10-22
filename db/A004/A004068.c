/*
 * A004068  Number of atoms in a decahedron with n shells.
 * https://oeis.org/A004068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*binomial(n+1,3) + n
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1727, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t x, y, z;
    seq_t b;
    if (n == 0) return 0;
    x = n - 1; y = n; z = n + 1;
    /* divide out 3 and 2 from the factors before multiplying */
    if (x % 3 == 0) x /= 3;
    else if (y % 3 == 0) y /= 3;
    else z /= 3;
    if (x % 2 == 0) x /= 2;
    else y /= 2;
    b = (seq_t)x * (seq_t)y * (seq_t)z;
    return 5 * b + (seq_t)n;
}
