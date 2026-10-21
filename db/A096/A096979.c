/*
 * A096979  Sum of the areas of the first n+1 Pell triangles.
 * https://oeis.org/A096979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-3) + a(n-4), with a(0)=0, a(1)=1,
 *            a(2)=6, a(3)=36
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)36;

    /* Sliding window of the last four terms */
    seq_t v[4];
    v[0] = (seq_t)0;   /* a(0) */
    v[1] = (seq_t)1;   /* a(1) */
    v[2] = (seq_t)6;   /* a(2) */
    v[3] = (seq_t)36;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t t1 = v[(i - 1) & 3] * (seq_t)6;          /* 6*a(n-1) */
        seq_t t2 = v[(i - 3) & 3] * (seq_t)6;          /* 6*a(n-3) */
        seq_t newv = t1 - t2 + v[(i - 4) & 3];          /* a(n) = 6*a(n-1) - 6*a(n-3) + a(n-4) */
        v[i & 3] = newv;
    }

    return v[n & 3];
}
