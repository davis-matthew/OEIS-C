/*
 * A072512  Product of all n - d, where 1 < d < n and d is a divisor of n.
 * https://oeis.org/A072512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ∏_{d|n,1<d<n} (n - d)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 35, u64 n <= 107,
 *            u128 n <= 239, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    seq_t ans = (seq_t)1;
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            ans *= ((seq_t)n - (seq_t)i);
            idx_t d2 = n / i;
            if (d2 != i) {
                ans *= ((seq_t)n - (seq_t)d2);
            }
        }
    }
    return ans;
}
