/*
 * A229751  Number of defective 4-colorings of an n X 4 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly two mistakes, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 33*a(n-2) + 63*a(n-3) - 66*a(n-4) + 36*a(n-5)
 *            - 8*a(n-6) (for n > 11), with initial values a(1)=3, a(2)=422,
 *            a(3)=2347, a(4)=6561, a(5)=15075, a(6)=32548, a(7)=69198,
 *            a(8)=147376, a(9)=315786, a(10)=680124, a(11)=1468934
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 21, u64 n <= 51, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 11 terms, 0‑based index */
    static const seq_t init[11] = {
        3, 422, 2347, 6561, 15075, 32548,
        69198, 147376, 315786, 680124, 1468934
    };

    if (n <= 11)
        return init[n-1];

    /* keep the last six terms: a(n-6)..a(n-1) */
    seq_t p[6] = {
        init[5],  /* a6 */
        init[6],  /* a7 */
        init[7],  /* a8 */
        init[8],  /* a9 */
        init[9],  /* a10 */
        init[10]  /* a11 */
    };

    seq_t next = 0;
    for (idx_t k = 12; k <= n; ++k) {
        next = 9*p[5] - 33*p[4] + 63*p[3] - 66*p[2] + 36*p[1] - 8*p[0];
        /* shift window */
        p[0] = p[1];
        p[1] = p[2];
        p[2] = p[3];
        p[3] = p[4];
        p[4] = p[5];
        p[5] = next;
    }
    return next;
}
