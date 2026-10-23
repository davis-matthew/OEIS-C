/*
 * A100216  Relates row sums of Pascal's triangle to expansion of
 *          cos(x)/exp(x).
 * https://oeis.org/A100216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) Initial values: a(0) = 1,
 *            a(1) = 4, a(2) = 9
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)9;

    /* a0 = a(i-3), a1 = a(i-2), a2 = a(i-1) */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)9;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) = 4*a(i-1) - 6*a(i-2) + 4*a(i-3) */
        seq_t an = (a2 << 2)            /* 4 * a(i-1) */
                 - (a1 * (seq_t)6)     /* 6 * a(i-2) */
                 + (a0 << 2);          /* 4 * a(i-3) */

        /* Shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;  /* a(n) */
}
