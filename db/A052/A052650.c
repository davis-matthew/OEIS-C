/*
 * A052650  Expansion of e.g.f. 1/((1-2*x)*(1-x)^2).
 * https://oeis.org/A052650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * (4 * 2^n - 3 - n) (equivalently, a(n) = (4*2^n - 3 -
 *            n) * n! )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast paths for very small n */
    if (n == 0)
        return (seq_t)1;      /* a(0) = 1 */

    seq_t fact = (seq_t)1;    /* factorial */
    seq_t pow2 = (seq_t)1;    /* 2^n */

    /* Compute n! and 2^n in a single loop */
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        fact *= (seq_t)i;        /* factorial multiplication */
        pow2 <<= (idx_t)1;       /* multiply by 2 each step */
    }

    /* term = 4 * 2^n - 3 - n */
    seq_t term = (seq_t)4 * pow2 - (seq_t)3 - (seq_t)n;

    return fact * term;
}
