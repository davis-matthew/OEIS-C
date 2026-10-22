/*
 * A003829  Maximal number of unit circles through n points in plane, each
 *          circle containing 3 of the points.
 * https://oeis.org/A003829
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(3) = 1; a(4) = 4; a(5) = 4; a(n) = 4(n-4) for n >= 6
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 3) return 1;
    if (n == 4 || n == 5) return 4;
    return 4 * ((seq_t)n - 4);
}
