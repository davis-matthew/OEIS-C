/*
 * A061377  a(1) = 1, a(n+1) = numerator of the continued fraction [1; 2, 4,
 *          8, ..., 2^n].
 * https://oeis.org/A061377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(1)=1, a(2)=3, a(n)=2^(n-1)*a(n-1)+a(n-2) for n>=3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not defined, guard case */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a_prevprev = (seq_t)1; /* a(1) */
    seq_t a_prev     = (seq_t)3; /* a(2) */
    seq_t ai         = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i)
    {
        ai = ((seq_t)1 << (i-1)) * a_prev + a_prevprev;
        a_prevprev = a_prev;
        a_prev = ai;
    }
    return ai;
}
