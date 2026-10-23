/*
 * A042741  Denominators of continued fraction convergents to sqrt(901).
 * https://oeis.org/A042741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 60*a(n-1) + a(n-2); a(0) = 1, a(1) = 60
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)60;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)60;
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)60 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
