/*
 * A217959  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..2 n X 3 array.
 * https://oeis.org/A217959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 4*a(n-2) - 20*a(n-3) - 23*a(n-4) - 10*a(n-5)
 *            initial values: a(1)=9, a(2)=91, a(3)=835, a(4)=7765, a(5)=72257
 *            This implementation uses only the five most recent terms, so it
 *            works for any idx_t and seq_t (fixed-width integers or arbitrary
 *            precision integers). It requires no dyn...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* unused, defined for safety */

    /* initial 5 terms (indices 1..5) */
    static const seq_t init[5] = {
        (seq_t)9,     /* a(1) */
        (seq_t)91,    /* a(2) */
        (seq_t)835,   /* a(3) */
        (seq_t)7765,  /* a(4) */
        (seq_t)72257  /* a(5) */
    };

    if (n <= 5)
        return init[n - 1];                  /* return the cached value */

    /* variables to keep the last 5 terms */
    seq_t a1 = init[0];    /* a(n-5) */
    seq_t a2 = init[1];    /* a(n-4) */
    seq_t a3 = init[2];    /* a(n-3) */
    seq_t a4 = init[3];    /* a(n-2) */
    seq_t a5 = init[4];    /* a(n-1) */
    seq_t next;

    /* iterative computation from term 6 up to n */
    for (idx_t i = 6; i <= n; ++i)
    {
        next = (seq_t)10 * a5        /* 10*a(n-1) */
             - (seq_t)4  * a4        /* -4*a(n-2)  */
             - (seq_t)20 * a3        /* -20*a(n-3) */
             - (seq_t)23 * a2        /* -23*a(n-4) */
             - (seq_t)10 * a1;       /* -10*a(n-5) */

        /* shift the sliding window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;                           /* a(n) */
}
