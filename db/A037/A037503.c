/*
 * A037503  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,0,2.
 * https://oeis.org/A037503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-3) - 10*a(n-4) Initial values: a(1)=1,
 *            a(2)=10, a(3)=102, a(4)=1021
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    seq_t a1 = (seq_t)1;     /* a(1) */
    seq_t a2 = (seq_t)10;    /* a(2) */
    seq_t a3 = (seq_t)102;   /* a(3) */
    seq_t a4 = (seq_t)1021;  /* a(4) */

    if (n == (idx_t)1) return a1;
    if (n == (idx_t)2) return a2;
    if (n == (idx_t)3) return a3;
    if (n == (idx_t)4) return a4;

    /* iterative computation from a(5) onward */
    for (idx_t i = (idx_t)5; i <= n; ++i)
    {
        /* a = 10*a4 + a2 - 10*a1   (a4 = a(i-1), a2 = a(i-3), a1 = a(i-4)) */
        seq_t a = ((seq_t)10) * a4 + a2 - ((seq_t)10) * a1;
        /* shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a;
    }
    return a4;   /* a4 now holds a(n) */
}
