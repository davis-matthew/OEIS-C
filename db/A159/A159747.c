/*
 * A159747  If an array is made of columns of -nacci sequences, fibo-, tribo-
 *          etc. all starting w. 1,1,2 etc, the NW to SE diagonals can be
 *          extended by computation. The above is diagonal 10. See A159741 for
 *          details.
 * https://oeis.org/A159747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+8) – 576 for n ≥ 6; first five terms are fixed: 89,
 *            504, 1490, 3525, 7617
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) … a(5) */
    if (n <= 5) {
        static const seq_t tbl[5] = { 89, 504, 1490, 3525, 7617 };
        return tbl[n - 1];
    }

    /* a(n) = 2^(n+8) – 576 for n ≥ 6 */
    idx_t exp = n + 8;          /* exponent of 2 */
    seq_t val = (seq_t)1;

    /* multiply by 2, exp times */
    for (idx_t i = 0; i < exp; ++i)
        val <<= 1;              /* val = val * 2 */

    return val - (seq_t)576;
}
