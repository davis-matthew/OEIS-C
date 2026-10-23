/*
 * A019465  Multiply by 1, add 1, multiply by 2, add 2, etc., start with 2.
 * https://oeis.org/A019465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2; For n>0: if n is odd: a(n) = a(n-1) * ((n+1)/2) if n is
 *            even: a(n) = a(n-1) + (n/2)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 40, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    seq_t val = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i & 1U)          /* odd index: multiply */
        {
            seq_t factor = (seq_t)((i + 1) / 2);
            val = val * factor;
        }
        else                 /* even index: add */
        {
            seq_t add = (seq_t)(i / 2);
            val = val + add;
        }
    }
    return val;
}
