/*
 * A233014  Number of nX4 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally or vertically, top left element zero, and 1
 *          appearing before 2 in row major order.
 * https://oeis.org/A233014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the empirical recurrence a(n) = 46*a(n-1) -
 *            312*a(n-2) + 530*a(n-3) - 263*a(n-4) with initial values: a(1) =
 *            14, a(2) = 515, a(3) = 19643, a(4) = 750092
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Initial constants of the sequence */
    const seq_t a1_const = 14;
    const seq_t a2_const = 515;
    const seq_t a3_const = 19643;
    const seq_t a4_const = 750092;

    /* Handle the first four terms directly */
    if (n == 1) return a1_const;
    if (n == 2) return a2_const;
    if (n == 3) return a3_const;
    if (n == 4) return a4_const;

    /* For n >= 5 use the recurrence */
    seq_t a1 = a1_const;  /* a(n-4)  */
    seq_t a2 = a2_const;  /* a(n-3)  */
    seq_t a3 = a3_const;  /* a(n-2)  */
    seq_t a4 = a4_const;  /* a(n-1)  */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = (seq_t)46 * a4
                 - (seq_t)312 * a3
                 + (seq_t)530 * a2
                 - (seq_t)263 * a1;

        /* Shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return a4;  /* a(n) */
}
