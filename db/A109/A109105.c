/*
 * A109105  a(n) = (8*sqrt(5)/25)((sqrt(5) + 2)((15 + 5*sqrt(5))/2)^n +
 *          (sqrt(5) - 2)((15 - 5*sqrt(5))/2)^n).
 * https://oeis.org/A109105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1)-25*a(n-2) for n >= 3; a(1) = 40, a(2) = 520,
 *            offset 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)40;
    if (n == 2) return (seq_t)520;

    seq_t prev2 = (seq_t)40;   /* a(1) */
    seq_t prev1 = (seq_t)520;  /* a(2) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur   = 15 * prev1 - 25 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
