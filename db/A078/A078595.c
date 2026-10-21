/*
 * A078595  Number of pairs (x,y) 1<=x<=y<=n such that 1/x+1/y+1/n = 1/2.
 * https://oeis.org/A078595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y) : 1≤x≤y≤n , 1/x+1/y+1/n = 1/2}|
 * Range:     u8 n <= 36, u16 n <= 1071, u32 n <= 9758, u64 n <= 9758,
 *            u128 n <= 9758, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;                /* no solution for n=1,2 */

    /* n must be ≥3 to avoid division by zero in the loop below. */
    seq_t two_n = (seq_t)2 * n;        /* 2*n, fits in seq_t safely */
    /* minimal x such that denominator ((n-2)*x - 2n) is positive */
    idx_t min_x = (idx_t)(((seq_t)2 * n) / (seq_t)(n - 2) + 1);
    if (min_x == 0)      /* this happens only for n=2, already handled */
        min_x = 1;

    seq_t count = 0;
    for (idx_t x = min_x; x <= n; ++x)
    {
        seq_t den = (seq_t)(n - 2) * x - two_n;   /* ((n-2)*x - 2n) */
        if (den == 0)          /* would lead to division by zero */
            continue;
        seq_t num = two_n * x;   /* (2n)*x */
        if (num % den == 0)
        {
            seq_t y = num / den;
            if (y >= x && y <= n)
                ++count;
        }
    }
    return count;
}
