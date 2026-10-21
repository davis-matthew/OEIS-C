/*
 * A200752  Expansion of (-x^2 + 3*x - 1)/(x^3 - x^2 + 3*x - 1).
 * https://oeis.org/A200752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) + a(n-3), a(0)=1, a(1)=0, a(2)=0 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)0;
    seq_t a2 = (seq_t)0;
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t ai = 3 * a2 - a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
