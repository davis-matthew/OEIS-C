/*
 * A117109  Moebius transform of binomial(n+3, 4).
 * https://oeis.org/A117109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} μ(d) * binom(n/d + 3, 4) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 565, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9868 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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

/* binomial(n+3,4) = (n+3)(n+2)(n+1)n / 24   (n >= 0) */
static inline seq_t binom_nplus3_choose4(idx_t n)
{
    if (n == 0) return 0;
    seq_t a = (seq_t)(n + 3);
    seq_t b = (seq_t)(n + 2);
    seq_t c = (seq_t)(n + 1);
    seq_t d = (seq_t)n;
    seq_t res = a * b;
    res *= c;
    res *= d;
    return res / (seq_t)24;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; n≥1 in normal use */
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;
            int mu_d = mu_of(d);
            if (mu_d > 0)
                result += binom_nplus3_choose4(e);
            else if (mu_d < 0)
                result -= binom_nplus3_choose4(e);

            if (e != d)
            {
                int mu_e = mu_of(e);
                if (mu_e > 0)
                    result += binom_nplus3_choose4(d);
                else if (mu_e < 0)
                    result -= binom_nplus3_choose4(d);
            }
        }
    }
    return result;
}
