/*
 * A021006  Pisot sequence P(4,11), a(0)=4, a(1)=11, a(n+1) is the nearest
 *          integer to a(n)^2/a(n-1). Evidently satisfies a(n) =
 *          2*a(n-1)+2*a(n-2).
 * https://oeis.org/A021006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2), a(0) = 4, a(1) = 11
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)11;

    /* iterate the linear recurrence:
       ai = 2*a(i-1) + 2*a(i-2)
       implemented as additions only for maximum type compatibility */
    seq_t a0 = (seq_t)4;   /* corresponds to a(n-2) */
    seq_t a1 = (seq_t)11;  /* corresponds to a(n-1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* ai = a1 + a1 + a0 + a0  == 2*a1 + 2*a0 */
        ai = a1 + a1 + a0 + a0;
        a0 = a1;
        a1 = ai;
    }
    return a1; /* a(n) */
}
