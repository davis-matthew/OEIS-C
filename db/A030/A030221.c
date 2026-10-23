/*
 * A030221  Chebyshev even-indexed U-polynomials evaluated at sqrt(7)/2.
 * https://oeis.org/A030221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - a(n-2), with a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)6;

    /* iterative recurrence, O(n) which is fine for n <= 10000 */
    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)6;      /* a(1) */
    seq_t ai = (seq_t)0;      /* current a(i) */

    for (idx_t i = 2u; i <= n; ++i)
    {
        ai = (seq_t)5 * a1 - a0;   /* a(i) = 5*a(i-1) - a(i-2) */
        a0 = a1;
        a1 = ai;
    }

    return a1;  /* a(n) */
}
