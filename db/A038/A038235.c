/*
 * A038235  Bottom line of 4-wave sequence A038197, also bisection of A006357.
 * https://oeis.org/A038235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 15*a(n-2) + 7*a(n-3) - a(n-4) a(0) = 1, a(1)
 *            = 10, a(2) = 85, a(3) = 707
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)85;
    if (n == 3) return (seq_t)707;

    /* Hold the four most recent values:
       v0 = a(n-4), v1 = a(n-3), v2 = a(n-2), v3 = a(n-1) */
    seq_t v0 = (seq_t)1;      /* a(0) */
    seq_t v1 = (seq_t)10;     /* a(1) */
    seq_t v2 = (seq_t)85;     /* a(2) */
    seq_t v3 = (seq_t)707;    /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) = 10*v3 - 15*v2 + 7*v1 - v0
           Rearranged to avoid unsigned underflow:
           a(i) = (10*v3 + 7*v1) - (15*v2 + v0) */
        seq_t sum1 = 10 * v3 + 7 * v1;
        seq_t sum2 = 15 * v2 + v0;
        seq_t new_val = sum1 - sum2;

        /* Shift the window for the next iteration */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = new_val;
    }

    return v3;
}
