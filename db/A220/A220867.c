/*
 * A220867  Number of perfect matchings in the twisted graph (C_4 X
 *          C_n)_(1234).
 * https://oeis.org/A220867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 4*a(n-1) + 5*a(n-2)
 *            - 24*a(n-3) + 5*a(n-4) + 4*a(n-5) - a(n-6) for n ≥ 8, with
 *            initial terms a(1)=2, a(2)=14, a(3)=80, a(4)=194, a(5)=888,
 *            a(6)=2702, a(7)=11040
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* hard‑coded initial values for indices 1 … 7 */
    static const seq_t init[7] = {
        (seq_t)2,
        (seq_t)14,
        (seq_t)80,
        (seq_t)194,
        (seq_t)888,
        (seq_t)2702,
        (seq_t)11040
    };

    /* For n within the pre‑computed range, return the value directly. */
    if (n <= 7)
        return init[n - 1];

    /* Store the last six values needed for the recurrence.
       last[0] = a(n-6), last[1] = a(n-5), … , last[5] = a(n-1). */
    seq_t last[6];
    last[0] = init[1]; /* a(2)  */
    last[1] = init[2]; /* a(3)  */
    last[2] = init[3]; /* a(4)  */
    last[3] = init[4]; /* a(5)  */
    last[4] = init[5]; /* a(6)  */
    last[5] = init[6]; /* a(7)  */

    /* Generate up to the requested index using the homogeneous
       recurrence that holds for n ≥ 8. */
    for (idx_t cur = 8; cur <= n; ++cur) {
        seq_t next = 4 * last[5]          /*  a(cur-1) */
                  + 5 * last[4]          /*  a(cur-2) */
                  - 24 * last[3]         /*  a(cur-3) */
                  + 5 * last[2]          /*  a(cur-4) */
                  + 4 * last[1]          /*  a(cur-5) */
                  - last[0];             /*  a(cur-6) */

        /* Shift the window: discard the oldest, shift left, insert new value. */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = next;
    }

    return last[5]; /* a(n) */
}
