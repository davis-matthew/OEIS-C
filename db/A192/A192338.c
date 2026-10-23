/*
 * A192338  Constant term of the reduction of n-th polynomial at A157751 by
 *          x^2->x+2.
 * https://oeis.org/A192338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 10*a(n-3) - 4*a(n-4)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=18 */

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (out of range) */
    if (n == 0)
        return 0;

    /* Base cases */
    static const seq_t base[4] = { (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)18 };
    if (n <= 4)
        return base[n - 1];

    /* Variables to keep the last four terms:
     * a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1) */
    seq_t a1 = base[0];
    seq_t a2 = base[1];
    seq_t a3 = base[2];
    seq_t a4 = base[3];

    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = 4 * a4 + a3 - 10 * a2 - 4 * a1;
        /* Shift terms for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
