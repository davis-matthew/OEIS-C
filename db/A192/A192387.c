/*
 * A192387  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 12*a(n-2) - 8*a(n-3) - 16*a(n-4) with initial
 *            terms a(1)=0, a(2)=2, a(3)=4, a(4)=32
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* if n is within the first four terms, return directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)32;

    /* iterative recurrence computation */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t a3 = (seq_t)4;   /* a(3) */
    seq_t a4 = (seq_t)32;  /* a(4) */
    seq_t ai;

    for (idx_t i = 5; i <= n; ++i)
    {
        ai = 2 * a4 + 12 * a3 - 8 * a2 - 16 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}
