/*
 * A192306  1-sequence of reduction of (2n) by x^2 -> x+1.
 * https://oeis.org/A192306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4) + a(n-5) (n ≥ 7)
 *            with initial values: a(1)=0, a(2)=4, a(3)=10, a(4)=26, a(5)=56,
 *            a(6)=116
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {                /* undefined offset; return 0 safely */
        return (seq_t)0;
    }
    idx_t idx = n - 1;           /* 0‑based index: idx=0 → a(1) */

    /* initial terms a(1) … a(6) */
    static const seq_t init[6] = {
        (seq_t)0,
        (seq_t)4,
        (seq_t)10,
        (seq_t)26,
        (seq_t)56,
        (seq_t)116
    };
    if (idx < 6) {               /* return the known term directly */
        return init[idx];
    }

    /* rolling window of the last 5 terms required for the recurrence */
    seq_t w[5];
    w[0] = init[1];  /* a(2) */
    w[1] = init[2];  /* a(3) */
    w[2] = init[3];  /* a(4) */
    w[3] = init[4];  /* a(5) */
    w[4] = init[5];  /* a(6) */

    seq_t cur = init[5];           /* placeholder for a(6) */
    for (idx_t i = 6; i <= idx; ++i) {
        cur = 3 * w[4] - w[3] - 3 * w[2] + w[1] + w[0];
        /* shift the window for the next iteration */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = cur;
    }
    return cur;                   /* cur holds a(n) for the requested n */
}
