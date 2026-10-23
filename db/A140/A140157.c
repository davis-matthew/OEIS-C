/*
 * A140157  a(1)=1, a(n) = a(n-1) + n^4 if n odd, a(n) = a(n-1) + n^0 if n is
 *          even.
 * https://oeis.org/A140157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + (n^4 if n odd, else 1) for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 132, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* n < 1 is undefined; return 0 for safety */
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        if ((i & 1) != 0)   /* odd n */
        {
            seq_t v = (seq_t)i;
            res += v * v * v * v;   /* i^4 */
        }
        else                /* even n */
        {
            res += (seq_t)1;
        }
    }
    return res;
}
