/*
 * A195167  a(n) = 3*n - floor(n*r/2), where r=(1+sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A195167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - floor(n*r/2) with r = (1+sqrt(5))/2 (golden ratio)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 6809, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Supporting integer square root for arbitrary-size seq_t */
static seq_t
isqrt_seq_t(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t ans = 0;
    while (low <= high)
    {
        seq_t mid = (low + high) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
        {
            ans = mid;
            low = mid + (seq_t)1;
        }
        else
        {
            high = mid - (seq_t)1;
        }
    }
    return ans;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;                 /* cast index to seq_t */

    /* Compute floor((n + floor(sqrt(5*n*n)))/4) which equals floor(n*phi/2) */
    seq_t n_sq = nn * nn;
    seq_t f5 = (seq_t)5;
    seq_t inside = f5 * n_sq;            /* 5*n*n */
    seq_t sqrt_val = isqrt_seq_t(inside);/* floor(sqrt(5*n*n)) */
    seq_t floor_term = (nn + sqrt_val) / (seq_t)4;

    /* a(n) = 3*n - floor_term */
    seq_t result = (seq_t)3 * nn - floor_term;
    return result;
}
