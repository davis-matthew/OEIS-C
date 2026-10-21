/*
 * A224042  Number of 6 X n 0..1 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A224042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 3*n^5 + 175*n^4 + 1905*n^3 + 19984*n^2 + 134172*n
 *            + 4320)/720 a(1) = 64, a(2) = 377, a(3) = 848, a(4) = 1422
 *            (valid for n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 45, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t first[4] = { 64, 377, 848, 1422 };
    if (n <= 4 && n >= 1) return first[n - 1];

    /* use formula for n > 4 */
    seq_t n0 = (seq_t)n;
    seq_t n2 = n0 * n0;
    seq_t n3 = n2 * n0;
    seq_t n4 = n3 * n0;
    seq_t n5 = n4 * n0;
    seq_t n6 = n5 * n0;

    seq_t numerator = n6
                    + 3 * n5
                    + 175 * n4
                    + 1905 * n3
                    + 19984 * n2
                    + 134172 * n0
                    + 4320;
    return numerator / 720;
}
