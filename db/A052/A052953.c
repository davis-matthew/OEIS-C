/*
 * A052953  Expansion of 2*(1-x-x^2)/((1-x)*(1+x)*(1-2*x)).
 * https://oeis.org/A052953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=2, a(1)=2; a(n)=a(n-1)+2*a(n-2)-2 (n≥2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0u || n == 1u)
        return (seq_t)2;

    /* a(n-1) and a(n-2) for the current iteration */
    seq_t a_prev = (seq_t)2; /* a(1)  */
    seq_t a_prev2 = (seq_t)2; /* a(0)  */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = a_prev;               /* a(i-1)  */
        cur += (seq_t)2 * a_prev2;   /* + 2*a(i-2) */
        cur -= (seq_t)2;             /* - 2       */

        /* Advance the window for next iteration */
        a_prev2 = a_prev;
        a_prev  = cur;
    }
    return cur;
}
