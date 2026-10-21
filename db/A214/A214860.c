/*
 * A214860  First differences of round(n*sqrt(3)) (A022847).
 * https://oeis.org/A214860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(3)) - round((n-1)*sqrt(3))
 * Range:     u8 n <= 9, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt(seq_t x)
{
    /* Integer square root: return floor(sqrt(x)) using only integer ops */
    seq_t lo = 0, hi = 1;
    /* Increase hi until hi*hi > x   (i.e., hi > x/hi) */
    while (x / hi >= hi)
        hi <<= 1;
    lo = hi >> 1;
    while (lo + 1 < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        if (x / mid >= mid) /* mid*mid <= x  */
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

static seq_t round_sqrt3(idx_t n)
{
    /* Return round(n*sqrt(3)) using integer arithmetic only */
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n * (seq_t)n;       /* n² */
    seq_t s  = (seq_t)3 * nn;             /* 3*n² */
    seq_t m  = isqrt(s);                  /* floor(sqrt(3*n²)) */
    seq_t m2 = m + 1;
    seq_t dist1 = s - m * m;              /* distance to m   */
    seq_t dist2 = m2 * m2 - s;            /* distance to m+1 */
    /* round to nearest integer; ties (dist1==dist2) round up */
    return (dist2 <= dist1) ? m2 : m;
}

seq_t a(idx_t n)
{
    /* a(1) is the first term; defined for n >= 1 */
    if (n == 0)
        return 0; /* out of range, harmless */
    seq_t rn   = round_sqrt3(n);
    seq_t rn_1 = round_sqrt3(n - 1);
    return rn - rn_1;
}
