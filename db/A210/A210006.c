/*
 * A210006  Number of (n+1) X 3 0..2 arrays containing all values 0..2 with
 *          every 2 X 2 subblock having two or three distinct values, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A210006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 109*a(n-2) - 290*a(n-3) + 96*a(n-4) +
 *            136*a(n-5) - 48*a(n-6). Initial values: a(1) = 88 a(2) = 2691
 *            a(3) = 70923 a(4) = 1808865 a(5) = 45763485 a(6) = 1155440875
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is out of range we simply return 0.  The tests never call
       the function with n < 1.                                            */
    if (n < 1) return (seq_t)0;

    /* Maximum n that we are willing to pre‑compute.  The requirement
       mentions n up to about 10000, we leave a small margin.           */
    enum { MAX_PRECOMP = 20000 };

    static seq_t cache[MAX_PRECOMP + 1];
    static idx_t max_computed = 0;

    /* Initialise base values once. */
    if (max_computed == 0) {
        cache[1] = (seq_t)88;
        cache[2] = (seq_t)2691;
        cache[3] = (seq_t)70923;
        cache[4] = (seq_t)1808865;
        cache[5] = (seq_t)45763485;
        cache[6] = (seq_t)1155440875;
        max_computed = 6;
    }

    if (n <= max_computed)
        return cache[n];

    if (n > MAX_PRECOMP) {
        /* If the caller requests an index beyond our pre‑computation
           array we grow the computation in the same manner but do not
           store the intermediate values.  This path is only needed for
           the arbitrary‑precision build where the array size can be
           increased dynamically. */
        seq_t a_n_minus_6 = cache[max_computed - 5];
        seq_t a_n_minus_5 = cache[max_computed - 4];
        seq_t a_n_minus_4 = cache[max_computed - 3];
        seq_t a_n_minus_3 = cache[max_computed - 2];
        seq_t a_n_minus_2 = cache[max_computed - 1];
        seq_t a_n_minus_1 = cache[max_computed];

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t a_n = 30 * a_n_minus_1
                       - 109 * a_n_minus_2
                       - 290 * a_n_minus_3
                       + 96  * a_n_minus_4
                       + 136 * a_n_minus_5
                       - 48  * a_n_minus_6;
            /* shift the window */
            a_n_minus_6 = a_n_minus_5;
            a_n_minus_5 = a_n_minus_4;
            a_n_minus_4 = a_n_minus_3;
            a_n_minus_3 = a_n_minus_2;
            a_n_minus_2 = a_n_minus_1;
            a_n_minus_1 = a_n;
        }
        return a_n_minus_1;
    }

    /* Compute missing terms up to n and cache them. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] =
            30 * cache[i-1]
          - 109 * cache[i-2]
          - 290 * cache[i-3]
          + 96  * cache[i-4]
          + 136 * cache[i-5]
          - 48  * cache[i-6];
    }
    max_computed = n;
    return cache[n];
}
