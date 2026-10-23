/*
 * A146964  a(n) = ((4 + sqrt(7))^n + (4 - sqrt(7))^n)/2.
 * https://oeis.org/A146964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((4 + sqrt(7))^n + (4 - sqrt(7))^n)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 9*a(n-2), with a(0) = 1, a(1) = 4 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 4;

    seq_t a0 = 1;   /* a(0) */
    seq_t a1 = 4;   /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i)
    {
        an = 8 * a1 - 9 * a0;  /* use the linear recurrence */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
