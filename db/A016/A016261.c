/*
 * A016261  Expansion of 1/((1-x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A016261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*10^(n+2) - 9^(n+3) + 1)/72
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=20, a(n)=19*a(n-1)-90*a(n-2)+1 for n>=2 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;
    seq_t a0 = 1;
    seq_t a1 = 20;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = 19 * a1 - 90 * a0 + 1;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
