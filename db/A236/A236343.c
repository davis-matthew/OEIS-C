/*
 * A236343  Expansion of (1 - x + 2*x^2 - x^3) / ((1 - x)^2 * (1 - x^3)) in
 *          powers of x.
 * https://oeis.org/A236343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) for n >= 5
 *            with initial values a(0)=1, a(1)=1, a(2)=3, a(3)=5, a(4)=6
 * Range:     u8 n <= 36, u16 n <= 624, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Base values for n = 0..4 */
    if (n < 5) {
        static const seq_t base[5] = {1, 1, 3, 5, 6};
        return base[n];
    }

    /* a5 holds a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) respectively */
    seq_t a5[5] = {1, 1, 3, 5, 6};

    for (idx_t i = 5; i <= n; ++i) {
        /* Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) */
        seq_t next = 2 * a5[4] - a5[3] + a5[2] - 2 * a5[1] + a5[0];

        /* Shift old values left: drop a(n-5), move others up */
        a5[0] = a5[1];
        a5[1] = a5[2];
        a5[2] = a5[3];
        a5[3] = a5[4];
        a5[4] = next;
    }

    return a5[4];
}
