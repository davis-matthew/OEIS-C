/*
 * A180847  a(n) = (27^n-4^n)/23.
 * https://oeis.org/A180847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (27^n-4^n)/23 (OEIS A180847)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1,
 * a(n) = 31*a(n-1) - 108*a(n-2)  for n ≥ 2 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i)
    {
        current = 31 * prev1 - 108 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
