/*
 * A219810  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or diagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 n X 2 array.
 * https://oeis.org/A219810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 6 n^3 + 41 n^2 - 36 n + 12) / 12 for n > 1, a(1) =
 *            3 Recurrence: a(n) = 5 a(n-1) - 10 a(n-2) + 10 a(n-3) - 5 a(n-4)
 *            + a(n-5) for n > 6 Initial terms: a(1)=3, a(2)=6, a(3)=16,
 *            a(4)=33, a(5)=61, a(6)=106
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle known small indices directly */
    if (n <= 6) {
        static const seq_t init[7] = {0, 3, 6, 16, 33, 61, 106};
        return init[n];
    }

    /* Maintain the last five values: a(n-5) … a(n-1) */
    seq_t last[5];
    last[0] = (seq_t)6;   /* a(2) */
    last[1] = (seq_t)16;  /* a(3) */
    last[2] = (seq_t)33;  /* a(4) */
    last[3] = (seq_t)61;  /* a(5) */
    last[4] = (seq_t)106; /* a(6) */

    for (idx_t k = 7; k <= n; ++k) {
        seq_t xn = (seq_t)5 * last[4] - (seq_t)10 * last[3]
                    + (seq_t)10 * last[2] - (seq_t)5 * last[1] + last[0];

        /* shift the window: drop a(n-5) and add a(n) */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = xn;
    }

    return last[4]; /* this is a(n) */
}
