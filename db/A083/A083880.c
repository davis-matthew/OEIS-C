/*
 * A083880  a(0)=1, a(1)=5, a(n) = 10*a(n-1) - 23*a(n-2), n >= 2.
 * https://oeis.org/A083880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t prev2 = 1;      /* a(0) */
    seq_t prev1 = 5;      /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * 10 - prev2 * 23;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
