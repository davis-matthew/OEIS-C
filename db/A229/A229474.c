/*
 * A229474  Number of defective 4-colorings of an n X 3 0..3 array connected
 *          horizontally, antidiagonally and vertically with exactly one
 *          mistake, and colors introduced in row-major 0..3 order.
 * https://oeis.org/A229474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 65*a(n-2) + 112*a(n-3) - 64*a(n-4) for n>5, *
 *            a(1) = 2, a(2) = 42, a(3) = 420, a(4) = 3328, a(5) = 23964
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handles the first five terms directly */
    static const seq_t init[5] = { (seq_t)2,
                                  (seq_t)42,
                                  (seq_t)420,
                                  (seq_t)3328,
                                  (seq_t)23964 };

    if (n == 0) /* offset 1: a(1) is the first term; return 0 for n<1 */
        return (seq_t)0;
    if (n <= 5)
        return init[n - 1];

    /* Use a sliding window of the last four values. */
    seq_t a_n1 = init[4];                       /* a(n-1) */
    seq_t a_n2 = init[3];                       /* a(n-2) */
    seq_t a_n3 = init[2];                       /* a(n-3) */
    seq_t a_n4 = init[1];                       /* a(n-4) */
    seq_t cur  = 0;

    for (idx_t i = 6; i <= n; ++i) {
        cur = (seq_t)14 * a_n1 - (seq_t)65 * a_n2 + (seq_t)112 * a_n3
              - (seq_t)64 * a_n4;

        /* Shift the window: a_n4 <- a_n3, a_n3 <- a_n2, a_n2 <- a_n1,
           a_n1 <- cur (new a(i)) */
        a_n4 = a_n3;
        a_n3 = a_n2;
        a_n2 = a_n1;
        a_n1 = cur;
    }

    return cur;
}
