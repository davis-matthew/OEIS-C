/*
 * A099255  Expansion of g.f. (7+6*x-6*x^2-3*x^3)/((x^2+x-1)*(x^2-x-1)).
 * https://oeis.org/A099255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) – a(n-4) for n ≥ 4 Initial values: a(0)=7,
 *            a(1)=6, a(2)=15, a(3)=15
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    switch (n) {
        case 0: return (seq_t)7;
        case 1: return (seq_t)6;
        case 2: return (seq_t)15;
        case 3: return (seq_t)15;
    }

    /* Use a circular buffer of the last four values:
       t0 = a(n-4), t1 = a(n-3), t2 = a(n-2), t3 = a(n-1) */
    seq_t t0 = (seq_t)7;   /* a(0)   */
    seq_t t1 = (seq_t)6;   /* a(1)   */
    seq_t t2 = (seq_t)15;  /* a(2)   */
    seq_t t3 = (seq_t)15;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = (seq_t)3 * t2 - t0;  /* a(i) = 3*a(i-2) - a(i-4) */

        /* Shift the buffer for next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = val;
    }

    return t3;
}
