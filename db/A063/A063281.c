/*
 * A063281  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          8 ).
 * https://oeis.org/A063281
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-2) + a(n-4) - a(n-6) for n ≥ 8 a(2) = –1, a(3) = 0,
 *            a(4) = 1, a(5) = 3, a(6) = 3, a(7) = 5. The sequence is indexed
 *            starting at 2 (offset 2)
 * Range:     i8 n <= 67, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init[6] = {
    (seq_t)-1, /* a(2) */
    (seq_t)0,  /* a(3) */
    (seq_t)1,  /* a(4) */
    (seq_t)3,  /* a(5) */
    (seq_t)3,  /* a(6) */
    (seq_t)5   /* a(7) */
};

seq_t a(idx_t n)
{
    /* Behaviour for n < 2 is irrelevant; return 0. */
    if (n < 2)
        return 0;

    /* Return one of the initial values directly. */
    if (n <= 7)
        return init[n - 2];

    /* last[0] … last[5] hold a_{k-6}, … , a_{k-1}
       for the current step k (starting with k = 8). */
    seq_t last[6];
    last[0] = init[0]; /* a(2) */
    last[1] = init[1]; /* a(3) */
    last[2] = init[2]; /* a(4) */
    last[3] = init[3]; /* a(5) */
    last[4] = init[4]; /* a(6) */
    last[5] = init[5]; /* a(7) */

    seq_t cur = 0;
    for (idx_t k = 8; k <= n; ++k) {
        /* a(k) = a(k-2) + a(k-4) – a(k-6) */
        cur = last[4] + last[2] - last[0];

        /* Shift the window for the next iteration. */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = cur;
    }

    return cur;
}
