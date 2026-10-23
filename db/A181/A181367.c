/*
 * A181367  Number of 2-compositions of n containing at least one 0 entry. A
 *          2-composition of n is a nonnegative matrix with two rows, such
 *          that each column has at least one nonzero entry and whose entries
 *          sum up to n.
 * https://oeis.org/A181367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) -10*a(n-2) +4*a(n-3) for n >= 5 with initial
 *            values a(1)=2, a(2)=6, a(3)=22, a(4)=78
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* not used, offset starts at 1 */
    if (n == 1)
        return 2;
    if (n == 2)
        return 6;
    if (n == 3)
        return 22;
    if (n == 4)
        return 78;

    /* constants used in the recurrence */
    const seq_t SIX  = 6;
    const seq_t TEN  = 10;
    const seq_t FOUR = 4;

    /* a1 = a(2), a2 = a(3), a3 = a(4) – the three previous terms */
    seq_t a1 = 6;      /* a(2) */
    seq_t a2 = 22;     /* a(3) */
    seq_t a3 = 78;     /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t term = SIX * a3;   /* 6*a(n-1) */
        term -= TEN * a2;        /* -10*a(n-2) */
        term += FOUR * a1;       /* +4*a(n-3) */
        a1 = a2;
        a2 = a3;
        a3 = term;
    }
    return a3;
}
