/*
 * A075988  Number of integers k satisfying 1 <= k <= n and 0 < frac(n/k) <
 *          1/2, where frac(n/k) is the fractional part of n/k; i.e.,
 *          frac(n/k) = n/k - floor(n,k).
 * https://oeis.org/A075988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k | 1 ≤ k ≤ n, 0 < n % k < k/2 }| (equivalent to a(n)
 *            = Σ_{k=1..n} (ceil(n/k) - round(n/k)) )
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t r = n % k;
        if (r && r < ((k >> 1) + (k & 1)))  /* r < ceil(k/2) = (k>>1)+(k&1) */
            ++count;
    }
    return count;
}
