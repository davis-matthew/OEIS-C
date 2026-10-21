/*
 * A052678  Expansion of e.g.f. x^3/(1-3*x).
 * https://oeis.org/A052678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n <= 2, a(3) = 6, a(n) = 3*n*a(n-1) for n > 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0), a(1), a(2) are zero */
    if (n <= 2U)
        return (seq_t)0;

    /* a(3) is 6 */
    if (n == 3U)
        return (seq_t)6;

    /* compute iteratively with the recurrence:
       a(k) = 3*k*a(k-1)   for k >= 4 */
    seq_t value = (seq_t)6;          /* a(3) */
    for (idx_t k = 4; k <= n; ++k)
        value *= (seq_t)(3ULL * k);

    return value;
}
