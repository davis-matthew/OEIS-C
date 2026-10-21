/*
 * A097108  If a geodesic dome is made by dividing each triangle of an
 *          icosahedron into n^2 identical equilateral triangles and the
 *          vertices of those newly created triangles are pushed out from the
 *          center to lie on the surface of the sphere in which the
 *          icosahedron is inscribed, then this sequence gives the...
 * https://oeis.org/A097108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 3n)/6 if n mod 3 == 0 (n+1)^2 /4 if n mod 6 == 1
 *            or 5 n*(n+2) /4 otherwise
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence is defined for n ≥ 1; 0 is unused */

    if ((n % 3) == 0) {
        seq_t nn = (seq_t)n;
        return (nn * nn + 3 * nn) / 6;
    }

    if ((n % 6) == 1 || (n % 6) == 5) {
        seq_t np1 = (seq_t)(n + 1);
        return (np1 * np1) / 4;
    }

    /* n % 6 == 2 or 4 */
    seq_t nn = (seq_t)n;
    return (nn * (nn + 2)) / 4;
}
