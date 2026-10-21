/*
 * A099163  Expansion of (1-2*x^2)/((1-2*x)*(1+x-x^2)).
 * https://oeis.org/A099163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) Initial values: a(0) = 1,
 *            a(1) = 1, a(2) = 2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Use only the last three computed values to save memory */
    seq_t t0 = (seq_t)1;   /* a(n-3) */
    seq_t t1 = (seq_t)1;   /* a(n-2) */
    seq_t t2 = (seq_t)2;   /* a(n-1) */

    /* Iterate up to n */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = t2 + (seq_t)3 * t1 - (seq_t)2 * t0;
        t0 = t1;
        t1 = t2;
        t2 = next;
    }
    return t2;
}
