/*
 * A077249  Bisection (odd part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2), with a(0)=2 and a(1)=21 (via a(-1) =
 *            -1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly, avoiding the use of a(-1). */
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)21;

    /* Iteratively apply the linear recurrence. */
    seq_t a0 = (seq_t)2;   /* a(0)   */
    seq_t a1 = (seq_t)21;  /* a(1)   */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a2 = (seq_t)10 * a1 - a0;  /* a(i) = 10*a(i-1) - a(i-2) */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
