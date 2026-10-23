/*
 * A168666  a(n) = n^7*(n^9 + 1)/2.
 * https://oeis.org/A168666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7*(n^9 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 16, u128 n <= 267,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_uint(seq_t base, int exp)
{
    seq_t res = 1;
    for (int i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (sn == 0) return 0;

    if ((sn & 1) == 0)                     /* n even: divide n^7 first */
    {
        seq_t t1   = pow_uint(sn, 7);       /* n^7  */
        seq_t t1d2 = t1 >> 1;              /* n^7 / 2 */
        seq_t t2   = pow_uint(sn, 9);       /* n^9  */
        t2 += 1;                            /* n^9 + 1 */
        return t1d2 * t2;
    }
    else                                   /* n odd: divide (n^9+1) first */
    {
        seq_t t1 = pow_uint(sn, 7);          /* n^7  */
        seq_t t2 = pow_uint(sn, 9);          /* n^9  */
        t2 += 1;                              /* n^9 + 1 */
        seq_t t2d2 = t2 >> 1;               /* (n^9 + 1) / 2 */
        return t1 * t2d2;
    }
}
