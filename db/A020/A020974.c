/*
 * A020974  Expansion of 1/((1-7*x)*(1-10*x)*(1-12*x)).
 * https://oeis.org/A020974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 274*a(n-2) + 840*a(n-3), * a(0)=1, a(1)=29,
 *            a(2)=567
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)567;

    /* Three most recent values */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)29;  /* a(n-2) */
    seq_t a2 = (seq_t)567; /* a(n-1) */
    seq_t ai;

    /* Iterate up to n */
    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 29*a(i-1) - 274*a(i-2) + 840*a(i-3) */
        /* Compute in an order that avoids underflow for unsigned types */
        ai = 29 * a2 - 274 * a1 + 840 * a0;
        /* Shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
