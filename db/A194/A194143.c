/*
 * A194143  Sum{floor(j*(3+sqrt(3))/2) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for (3+sqrt(3))/2.
 * https://oeis.org/A194143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1..n} floor(j*(3+sqrt(3))/2)
 * Range:     u8 n <= 9, u16 n <= 215, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root (floor) implemented via binary search.
   Works for any unsigned integer type used as seq_t. */
static seq_t seq_isqrt(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo < hi)
    {
        seq_t mid = (lo + hi + 1) >> 1;            /* (lo+hi+1)/2 */
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j)
    {
        seq_t tj   = (seq_t)j;                     /* j as seq_t */
        seq_t jj   = tj * tj;                      /* j²          */
        seq_t sq   = seq_isqrt((seq_t)3 * jj);     /* floor(j*sqrt(3))  */
        seq_t term = ((seq_t)3 * tj + sq) / (seq_t)2; /* floor(j*α)          */
        sum += term;
    }
    return sum;
}
