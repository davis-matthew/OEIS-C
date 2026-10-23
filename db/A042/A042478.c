/*
 * A042478  Numerators of continued fraction convergents to sqrt(767).
 * https://oeis.org/A042478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A042478: Numerators of continued fraction convergents to sqrt(767)
    a(0)=27; for n>=1, a(n)=c[n]*a(n-1)+a(n-2) where c[n] are
    the continued fraction partial quotients of sqrt(767) after the
    initial term 27, repeating with period 12: 1,2,3,1,1,1,1,1,3,2,1,54. */

seq_t a(idx_t n)
{
    /* Trivial case */
    if (n == 0)
        return (seq_t)27;

    /* Periodic partial quotients (excluding the initial a0=27) */
    static const int period[12] =
        {1, 2, 3, 1, 1, 1, 1, 1, 3, 2, 1, 54};

    /* Recurrence initial values:
       p_{-1} = 1,  p_0 = 27   (a_0)  */
    seq_t prev2 = (seq_t)1;      /* p_{k-2} */
    seq_t curr  = (seq_t)27;     /* p_{k-1} originally p_0 */

    /* Compute successive numerators */
    for (idx_t i = 1; i <= n; ++i)
    {
        int ai = period[(i - 1) % 12];
        seq_t next = ((seq_t)ai) * curr + prev2;
        prev2 = curr;
        curr = next;
    }

    return curr;   /* p_n */
}
