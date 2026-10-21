/*
 * A072229  Witt index of the standard bilinear form <1,1,1,...,1> over the
 *          2-adic rationals.
 * https://oeis.org/A072229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 4*floor(n/7) + [0,0,0,0,1,2,3][n%7]
 * Range:     u8 n <= 447, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The offset array corresponds to the values for n mod 7 */
    static const seq_t add[7] = {0,0,0,0,1,2,3};
    return 4 * (seq_t)(n / 7) + add[n % 7];
}
