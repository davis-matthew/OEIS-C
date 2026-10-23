/*
 * A122012  G.f.: x^2*(3+3*x-2*x^2)/ ( (x^2-x-1) * (x^2+x-1)).
 * https://oeis.org/A122012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-4) Initial values: a(1)=0, a(2)=3, a(3)=3,
 *            a(4)=7
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices below the offset (they are undefined). */
    if (n == 0) return 0;          /* n == 0 is not in the OEIS range */

    /* Return the first four explicit terms directly. */
    if (n <= 4) {
        static const seq_t base[4] = {0, 3, 3, 7};
        return base[n - 1];
    }

    /* For n >= 5 compute using the recurrence with a sliding window. */
    seq_t c[4] = {0, 3, 3, 7};      /* c[0]=a(i-4), c[1]=a(i-3), c[2]=a(i-2), c[3]=a(i-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t newVal = 3 * c[2] - c[0];   /* a(i) = 3*a(i-2) - a(i-4) */

        /* Shift the window to the left for the next iteration. */
        c[0] = c[1];
        c[1] = c[2];
        c[2] = c[3];
        c[3] = newVal;
    }

    return c[3];   /* c[3] now holds a(n) */
}
