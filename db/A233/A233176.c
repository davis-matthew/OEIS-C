/*
 * A233176  Number of 3 X n 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally, diagonally or
 *          antidiagonally, top left element zero, and 1 appearing before 2 3
 *          and 4, and 2 appearing before 3 in row major order (unlabelled
 *          6-colorings with no clashing color pairs).
 * https://oeis.org/A233176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*a(n-1) + 64*a(n-2) - 3072*a(n-3) + 8192*a(n-4) (n > 5)
 *            with initial values: a(1)=11, a(2)=80, a(3)=2688, a(4)=78336,
 *            a(5)=2469888
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial five terms */
    static const seq_t init[5] = {
        (seq_t)11,
        (seq_t)80,
        (seq_t)2688,
        (seq_t)78336,
        (seq_t)2469888
    };

    if (n <= 5)
        return init[n - 1];

    /* Sliding window of the last four terms:
         a(n-4), a(n-3), a(n-2), a(n-1)  */
    seq_t a_n4 = init[1];   /* a(2) */
    seq_t a_n3 = init[2];   /* a(3) */
    seq_t a_n2 = init[3];   /* a(4) */
    seq_t a_n1 = init[4];   /* a(5) */

    seq_t a_n = a_n1;   /* will hold a(n) after each iteration */
    for (idx_t i = 6; i <= n; ++i) {
        a_n = a_n1 * (seq_t)32
            + a_n2 * (seq_t)64
            - a_n3 * (seq_t)3072
            + a_n4 * (seq_t)8192;

        /* shift the window forward */
        a_n4 = a_n3;
        a_n3 = a_n2;
        a_n2 = a_n1;
        a_n1 = a_n;
    }

    return a_n;
}
