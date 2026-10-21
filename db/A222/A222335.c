/*
 * A222335  Number of nX2 0..6 arrays with no entry increasing mod 7 by 6
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1), a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0; /* below the offset, return 0 */
    seq_t result = (seq_t)6;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)31;
    return result;
}
