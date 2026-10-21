/*
 * A090017  a(n) = 4*a(n-1) + 2*a(n-2) for n>1, a(0)=0, a(1)=1.
 * https://oeis.org/A090017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a0 = 0;   /* a(0) */
    seq_t a1 = 1;   /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i)
    {
        an = 4 * a1 + 2 * a0;   /* recurrence */
        a0  = a1;
        a1  = an;
    }
    return a1;
}
