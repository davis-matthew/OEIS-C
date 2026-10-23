/*
 * A147553  Numbers k such that k^2 divides k.k where dot "." means
 *          concatenation.
 * https://oeis.org/A147553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(1) = 1, a(2) = 143, a(3) = 142857143 For n > 3: a(n) =
 *            1000001*a(n-1) - 1000000*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)143;
    if (n == 3) return (seq_t)142857143;

    seq_t prev2 = (seq_t)143;            /* a(2)   */
    seq_t prev1 = (seq_t)142857143;      /* a(3)   */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i)
    {
        curr = (seq_t)1000001 * prev1 - (seq_t)1000000 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
