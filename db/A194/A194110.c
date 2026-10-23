/*
 * A194110  Sum{floor(j*sqrt(5)) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for sqrt(5).
 * https://oeis.org/A194110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1}^n floor(j*sqrt(5))
 * Range:     u8 n <= 14, u16 n <= 241, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for values up to 5*10000^2 = 5e8.
   Works with unsigned 64‑bit arithmetic and avoids overflow. */
static inline unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long low = 0, high = 65536, mid, ans = 0;   /* 65536^2 = 4.3e9 > 5e8 */
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned long long prod = mid * mid;
        if (prod <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        unsigned long long jj = (unsigned long long)j;
        unsigned long long temp = 5ULL * jj * jj;       /* j^2 * 5 */
        unsigned long long val = isqrt_u64(temp);        /* floor(j*sqrt(5)) */
        sum += (seq_t)val;
    }
    return sum;
}
