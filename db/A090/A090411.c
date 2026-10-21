/*
 * A090411  Expansion of g.f. (1-x)/(1-16*x).
 * https://oeis.org/A090411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(n) = 15 * 16^(n-1) for n >= 1 * Recurrence: a(0)=1,
 *            a(1)=15, a(n)=16*a(n-1) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t res = (seq_t)15;
    /* For n >= 2 multiply by 16 (n-1) times. */
    for (idx_t i = 2; i <= n; ++i)
        res *= (seq_t)16;
    return res;
}
