/*
 * A132106  a(n) = 1 + floor(sqrt(n)) + Sum_{i=1..n} floor(n/i).
 * https://oeis.org/A132106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + floor(sqrt(n)) + Sum_{i=1..n} floor(n/i)
 * Range:     u8 n <= 58, u16 n <= 7238, u32 n <= 9861, u64 n <= 9861,
 *            u128 n <= 9861, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_uint64(idx_t n)
{
    if (n == 0) return 0;
    idx_t lo = 1, hi = n, ans = 0;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        if (mid <= n / mid)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t sqrt_n = isqrt_uint64(n);
    seq_t sum = 0;

    for (idx_t i = 1; i <= n; )
    {
        idx_t q       = n / i;
        idx_t next_i  = n / q + 1;
        idx_t count   = next_i - i;
        sum += (seq_t)count * (seq_t)q;
        i = next_i;
    }

    return (seq_t)1 + (seq_t)sqrt_n + sum;
}
