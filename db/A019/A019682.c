/*
 * A019682  Expansion of 1/((1-4*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A019682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -9^(n+2)/5 + 5*10^(n+1)/3 + 2*4^(n+1)/15 Recurrence: a(n)
 *            = 23*a(n-1) - 166*a(n-2) + 360*a(n-3) (n > 2) Initial values:
 *            a(0)=1, a(1)=23, a(2)=363
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)363;

    /* iterate using the recurrence */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)23;  /* a(1) */
    seq_t a2 = (seq_t)363; /* a(2) */
    seq_t a3;              /* temporary for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a3 = 23 * a2 - 166 * a1 + 360 * a0;
        a0 = a1;        /* shift previous values */
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
