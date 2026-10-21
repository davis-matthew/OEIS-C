/*
 * A218746  a(n) = (43^n - 1)/42.
 * https://oeis.org/A218746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (43^n - 1)/42 Recurrence: a(0) = 0, a(1) = 1, a(n) =
 *            44*a(n-1) - 43*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t a0 = 0;   /* a(n-2)  */
    seq_t a1 = 1;   /* a(n-1)  */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Use the recurrence: a[n] = 44*a1 - 43*a0 */
        seq_t tmp = 44 * a1;  /* 44*a(n-1)   */
        tmp -= 43 * a0;       /* subtract 43*a(n-2) */
        a0 = a1;
        a1 = tmp;
    }
    return a1;
}
