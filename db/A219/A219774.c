/*
 * A219774  Number of 3 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          columns, 0..1 3 X n array.
 * https://oeis.org/A219774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n > 9, with initial values: a(1)=4, a(2)=6, a(3)=14, a(4)=33,
 *            a(5)=65, a(6)=114, a(7)=189, a(8)=298, a(9)=450
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 9 known initial values, index 0 corresponds to a(1) */
    static const seq_t init[9] = {4, 6, 14, 33, 65, 114, 189, 298, 450};

    if (n <= 9) /* exact answer for the first nine terms */
        return init[n - 1];

    /* Recurrence terms: p[0] = a(i-5), p[1] = a(i-4), ..., p[4] = a(i-1) */
    seq_t p[5];
    p[0] = init[4]; /* a(5) */
    p[1] = init[5]; /* a(6) */
    p[2] = init[6]; /* a(7) */
    p[3] = init[7]; /* a(8) */
    p[4] = init[8]; /* a(9) */

    for (idx_t i = 10; i <= n; ++i) {
        /* Compute next value using the recurrence
           a(i) = 5*a(i-1) - 10*a(i-2) + 10*a(i-3) - 5*a(i-4) + a(i-5) */
        seq_t pos = 5 * p[4] + 10 * p[2] + p[0];
        seq_t neg = 10 * p[3] + 5 * p[1];
        seq_t next = pos - neg;

        /* Shift window: drop a(i-5), keep a(i-4)..a(i) */
        p[0] = p[1];
        p[1] = p[2];
        p[2] = p[3];
        p[3] = p[4];
        p[4] = next;
    }

    return p[4]; /* a(n) */
}
