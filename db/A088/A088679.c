/*
 * A088679  a(n) = a(n-1)^2 * n / (n-1), n>1, a(0) = 0, a(1) = 1.
 * https://oeis.org/A088679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, n>1: a(n) = a(n-1)^2 * n / (n-1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function evaluates the recurrence directly, using only the
 * allowed integer operations. */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0u)
        return (seq_t)0;
    if (n == 1u)
        return (seq_t)1;

    /* start with a(1) */
    seq_t cur = (seq_t)1;

    /* iterate from 2 to n computing from the recurrence */
    for (idx_t i = 2u; i <= n; ++i)
    {
        seq_t tmp = cur * cur;        /* a(i-1)^2          */
        tmp *= (seq_t)i;              /* * n                */
        tmp /= (seq_t)(i - 1u);       /* / (i-1)            */
        cur = tmp;                    /* a(i) = tmp         */
    }
    return cur;                        /* a(n) for requested n */
}
