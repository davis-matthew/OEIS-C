/*
 * A077841  Expansion of (1-x)/(1-2*x-3*x^2-2*x^3).
 * https://oeis.org/A077841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=5, a(n)=2*a(n-1)+3*a(n-2)+2*a(n-3) for n≥3 */
seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)5;

    /* Keep only the last three values to compute the next one */
    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)1;  /* a(n-2) */
    seq_t a2 = (seq_t)5;  /* a(n-1) */
    seq_t ai;

    /* Compute sequentially up to n */
    for (idx_t i = 3; i <= n; ++i)
    {
        ai = (seq_t)2 * a2 + (seq_t)3 * a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
