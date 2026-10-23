/*
 * A027117  a(n) = A027113(n, n+4).
 * https://oeis.org/A027117
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A027113(n, n+4). Recurrence for n>9: * a(n) = 4*a(n-1) -
 *            5*a(n-2) + 2*a(n-3) - a(n-4) + 2*a(n-5) - a(n-6). * Initial
 *            values (offset 4): * a(4)=16, a(5)=59, a(6)=155, a(7)=352,
 *            a(8)=738, a(9)=1475
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 4.  For smaller n, return 0. */
    if (n < 4)
        return (seq_t)0;

    /* Initial terms a(4) … a(9). */
    static const seq_t init[6] = {
        (seq_t)16,   /* a(4) */
        (seq_t)59,   /* a(5) */
        (seq_t)155,  /* a(6) */
        (seq_t)352,  /* a(7) */
        (seq_t)738,  /* a(8) */
        (seq_t)1475  /* a(9) */
    };

    /* Return the initial term if n <= 9. */
    if (n <= 9)
        return init[n - 4];

    /* Sliding window over the last six values a(n-6) … a(n-1). */
    seq_t a0 = init[0];  /* a(n-6) */
    seq_t a1 = init[1];  /* a(n-5) */
    seq_t a2 = init[2];  /* a(n-4) */
    seq_t a3 = init[3];  /* a(n-3) */
    seq_t a4 = init[4];  /* a(n-2) */
    seq_t a5 = init[5];  /* a(n-1) */

    seq_t next;
    for (idx_t i = 10; i <= n; ++i) {
        /* Compute a(i) from the recurrence. */
        next = (seq_t)4 * a5
               - (seq_t)5 * a4
               + (seq_t)2 * a3
               - a2
               + (seq_t)2 * a1
               - a0;

        /* If we've reached the desired index, return the value. */
        if (i == n)
            return next;

        /* Slide the window: discard a0, shift others left, insert newest. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
