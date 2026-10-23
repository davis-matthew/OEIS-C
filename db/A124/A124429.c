/*
 * A124429  Antidiagonal sums of triangle A124428.
 * https://oeis.org/A124429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} C([(n-k)/2],k) * C([(n-k+1)/2],k)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 43, u64 n <= 83, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        idx_t m1 = (n - k) / 2;
        idx_t m2 = (n - k + 1) / 2;
        seq_t c1 = _binom(m1, k);
        seq_t c2 = _binom(m2, k);
        sum += c1 * c2;
    }
    return sum;
}
