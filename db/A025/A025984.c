/*
 * A025984  Expansion of 1/((1-2*x)*(1-4*x)*(1-10*x)*(1-12*x)).
 * https://oeis.org/A025984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 28*a(n-1) -
 *            260*a(n-2) + 896*a(n-3) - 960*a(n-4) (n ≥ 4) with initial
 *            values: a(0) = 1, a(1) = 28, a(2) = 524, a(3) = 8288
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;
    if (n == 2) return (seq_t)524;
    if (n == 3) return (seq_t)8288;

    /* Last four computed values: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)28;     /* a(n-3) */
    seq_t a2 = (seq_t)524;    /* a(n-2) */
    seq_t a3 = (seq_t)8288;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)28 * a3
                    - (seq_t)260 * a2
                    + (seq_t)896  * a1
                    - (seq_t)960 * a0;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
