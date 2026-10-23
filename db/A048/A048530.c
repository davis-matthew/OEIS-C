/*
 * A048530  Smallest oblong (promic) number containing exactly n 0's.
 * https://oeis.org/A048530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0; a(2)=600; a(4)=390000. For n>=3 and n!=4: if n is odd:
 *            a(n) = m*(m+1) with m = 10^((n+1)/2) if n is even: a(n) =
 *            m*(m+1) with m = 10^((n+2)/2) + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return (seq_t)600;
    if (n == 4) return (seq_t)390000;

    idx_t k;
    seq_t m = 1;

    if (n & 1)   /* odd n */
    {
        k = (n + 1) / 2;
    }
    else        /* even n (>=6) */
    {
        k = (n + 2) / 2;
    }

    /* compute 10^k */
    for (idx_t i = 0; i < k; ++i)
        m *= (seq_t)10;

    if ((n % 2) == 0)   /* even n >=6 */
        m = m + 1;

    return m * (m + 1);
}
