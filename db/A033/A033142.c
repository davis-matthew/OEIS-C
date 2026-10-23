/*
 * A033142  Base-6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,0.
 * https://oeis.org/A033142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((36/215)*6^n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) + a(n-3) - 6*a(n-4);
   Initial values: a(1)=1, a(2)=6, a(3)=36, a(4)=217 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined but safe for safety */

    /* Return the first four terms directly. */
    if (n <= 4) {
        static const seq_t init[4] = {
            (seq_t)1,
            (seq_t)6,
            (seq_t)36,
            (seq_t)217
        };
        return init[n - 1];
    }

    /* Keep the last four computed values. */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)6;   /* a(2) */
    seq_t a3 = (seq_t)36;  /* a(3) */
    seq_t a4 = (seq_t)217; /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute 6*a4 + a2 - 6*a1 using only additions. */
        seq_t tmp = a4 + a4 + a4 + a4 + a4 + a4; /* 6*a4 */
        tmp += a2;                                 /* + a2 */
        seq_t sub = a1 + a1 + a1 + a1 + a1 + a1;  /* 6*a1 */
        tmp -= sub;                                /* - 6*a1 */

        /* Shift the window: a1←a2, a2←a3, a3←a4, a4←tmp. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = tmp;
    }

    return a4;
}
