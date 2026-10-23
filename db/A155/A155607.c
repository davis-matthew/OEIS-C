/*
 * A155607  a(n) = 9^n + 3^n - 1.
 * https://oeis.org/A155607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + 3^n - 1 Recurrence: a(0)=1, a(1)=11, a(n)=12*a(n-1)
 *            - 27*a(n-2) - 16 for n>=2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)11;  /* a(1) */
    seq_t val;

    for (idx_t i = 2; i <= n; ++i)
    {
        val = (seq_t)12 * prev1 - (seq_t)27 * prev2 - (seq_t)16;
        prev2 = prev1;
        prev1 = val;
    }

    return prev1;
}
