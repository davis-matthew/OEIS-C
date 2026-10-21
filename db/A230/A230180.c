/*
 * A230180  Number of n X 4 0..2 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          2-x(i,j).
 * https://oeis.org/A230180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) - 97*a(n-2) + 89*a(n-3) - 18*a(n-4) + a(n-5)
 *            with initial values a(1)=9, a(2)=159, a(3)=3411, a(4)=73857,
 *            a(5)=1603431 (OEIS A230180)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases 1..5 */
    static const seq_t init[5] = {9, 159, 3411, 73857, 1603431};
    if (n <= 5) {
        return init[n - 1];
    }

    /* start with a(5), a(4), a(3), a(2), a(1) in reverse order */
    seq_t a1 = init[4];   /* a(i-1) */
    seq_t a2 = init[3];   /* a(i-2) */
    seq_t a3 = init[2];   /* a(i-3) */
    seq_t a4 = init[1];   /* a(i-4) */
    seq_t a5 = init[0];   /* a(i-5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t val = 26 * a1 - 97 * a2 + 89 * a3 - 18 * a4 + a5;

        /* update circular buffer for next step */
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = val;
    }
    return a1;
}
