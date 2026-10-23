/*
 * A122794  Connell (3,2)-sum sequence (partial sums of the (3,2)-Connell
 *          sequence).
 * https://oeis.org/A122794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-th triangular number)-n+(n-th partial sum of A122800)
 * Range:     u8 n <= 14, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* efficiently compute floor(sqrt(n)) working only with integer arithmetic */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0)
        {
            if (n == 0) return 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid)
        {
            if (mid + 1 > n / (mid + 1))
                return mid;
            low = mid + 1;
        }
        else
            high = mid - 1;
    }
    return low;
}

/* sum_{k=1}^n floor(sqrt(k-1))  */
static seq_t _sum_floor_sqrt(idx_t n)
{
    if (n == 0) return 0;
    idx_t max_m = _isqrt_u64(n - 1);
    seq_t sum = 0;
    for (idx_t m = 0; m <= max_m; ++m)
    {
        idx_t low  = m * m + 1;
        idx_t high = (m + 1) * (m + 1);
        if (high > n) high = n;
        if (low > n)  break;
        idx_t count = high - low + 1;
        sum += (seq_t)count * (seq_t)m;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, return 0 */
    seq_t nn        = (seq_t)n;
    seq_t tri_minus = nn * (nn - 1) / 2;       /* T(n)-n = n(n-1)/2   */
    seq_t sqsum     = _sum_floor_sqrt(n);      /* Σ floor(sqrt(k-1))  */
    seq_t S         = nn * nn - 2 * sqsum;     /* n² - 2Σ floor(sqrt) */
    return tri_minus + S;                       /* a(n) */
}
