/*
 * A136782  Number of primitive multiplex juggling sequences of length n, base
 *          state <2,2> and hand capacity 2.
 * https://oeis.org/A136782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 43*a(n-2) + 24*a(n-3) + 27*a(n-4), a(1)=1,
 *            a(2)=2, a(3)=16, a(4)=119
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset handling – a(0) is undefined, return 0 for safety */
    if (n == 0) return 0;

    /* initial values for n = 1..4 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)119;

    /* a(5) would be computed in the loop below */
    seq_t t1 = (seq_t)1;   /* a(n-4) at start of loop */
    seq_t t2 = (seq_t)2;   /* a(n-3) */
    seq_t t3 = (seq_t)16;  /* a(n-2) */
    seq_t t4 = (seq_t)119; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i)
    {
        /* compute a(i) using the recurrence */
        seq_t term = (seq_t)13 * t4
                   - (seq_t)43 * t3
                   + (seq_t)24 * t2
                   + (seq_t)27 * t1;

        /* shift window for next iteration */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = term;
    }

    return t4; /* t4 now holds a(n) */
}
