/*
 * A117353  Expansion of (1-x)/(1-3x+x^2+4x^3-4x^4).
 * https://oeis.org/A117353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 4*a(n-3) + 4*a(n-4), with a(0)=1,
 *            a(1)=2, a(2)=5, a(3)=9
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;

    /* recurrence iterated from 4 up to n */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)2;
    seq_t a2 = (seq_t)5;
    seq_t a3 = (seq_t)9;
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        next = (seq_t)3 * a3 - a2 - (seq_t)4 * a1 + (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
