/*
 * A056280  Number of primitive (aperiodic) word structures of length n which
 *          contain exactly four different symbols.
 * https://oeis.org/A056280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(d) * S(n/d,4) (S(m,4) = Stirling number of
 *            the second kind)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 489 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

static seq_t stirling4(idx_t n)
{
    seq_t s[5] = {0};
    s[0] = 1;                     /* S(0,0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ns[5] = {0};
        ns[0] = 0;                /* S(i,0) = 0 for i>0 */
        for (idx_t j = 1; j <= 4; ++j) {
            ns[j] = s[j] * (seq_t)j + s[j - 1];
        }
        for (idx_t j = 0; j <= 4; ++j) s[j] = ns[j];
    }
    return s[4];
}

seq_t a(idx_t n)
{
    seq_t sum_pos = 0, sum_neg = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;

        int mu1 = mu_of(d1);
        idx_t m1 = n / d1;
        if (mu1 == 1)
            sum_pos += stirling4(m1);
        else if (mu1 == -1)
            sum_neg += stirling4(m1);

        if (d1 != d2) {
            int mu2 = mu_of(d2);
            idx_t m2 = n / d2;   /* equals i */
            if (mu2 == 1)
                sum_pos += stirling4(m2);
            else if (mu2 == -1)
                sum_neg += stirling4(m2);
        }
    }
    return sum_pos - sum_neg;
}
