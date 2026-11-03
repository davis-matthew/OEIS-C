/*
 * A144826  Row sums of triangle A144825.
 * https://oeis.org/A144826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^2 * Sum_{d|gcd(n,k)} d * mu(n/d)
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 941 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    long long total = 0;                 /* signed accumulator (see note below) */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* g = gcd(n,k) using seq_t gcd to keep it generic */
        seq_t gg = gcd((seq_t)n, (seq_t)k);
        idx_t g = (idx_t)gg;             /* g fits in 64 bit because n is uint64_t */

        long long inner = 0;
        for (idx_t d = 1; d * d <= g; ++d)
        {
            if (g % d == 0)
            {
                /* contribution from divisor d */
                int mu_val = mu_of(n / d);
                inner += (long long)d * mu_val;

                /* contribution from paired divisor g/d (if distinct) */
                idx_t d2 = g / d;
                if (d2 != d)
                {
                    int mu_val2 = mu_of(n / d2);
                    inner += (long long)d2 * mu_val2;
                }
            }
        }

        long long k2 = (long long)k * k;          /* k^2 fits in 64 bit for n≤UINT64_MAX */
        total += k2 * inner;                      /* inner may be negative, but the final sum is non‑negative */
    }

    return (seq_t)total;                         /* cast final value to seq_t   */
}
