/*
 * A163920  Expansion of Sum_{k>0} k*(k+1)/2 * x^k / (1 - (-x)^k)^3.
 * https://oeis.org/A163920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n/4) * Σ_{d|n} (-1)^{n+d} (d+1)(n/d+1)
 * Range:     u8 n <= 8, u16 n <= 158, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* integer square root of an unsigned 64‑bit integer */
static idx_t u64_isqrt(idx_t x)
{
    if (x < 2) return x;
    long double rdl = sqrt((long double)x);
    idx_t r = (idx_t)rdl;
    /* adjust downwards if we overshot */
    while ((__int128)r * r > x) r--;
    /* adjust upwards */
    while ((__int128)(r + 1) * (r + 1) <= x) r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used for the official offset */
    idx_t sqrt_n = u64_isqrt(n);

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 1; i <= sqrt_n; i++) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* contribution from divisor d1 */
            int sign1 = ((n + d1) & 1) ? -1 : 1;
            seq_t term1 = (seq_t)(d1 + 1) * (seq_t)(n / d1 + 1);
            if (sign1 > 0) sum_pos += term1;
            else          sum_neg += term1;

            /* if d1 and d2 are distinct, process d2 */
            if (d1 != d2) {
                int sign2 = ((n + d2) & 1) ? -1 : 1;
                seq_t term2 = (seq_t)(d2 + 1) * (seq_t)(n / d2 + 1);
                if (sign2 > 0) sum_pos += term2;
                else          sum_neg += term2;
            }
        }
    }

    seq_t sum = sum_pos - sum_neg;        /* sum_pos >= sum_neg in all cases */
    seq_t num = (seq_t)n * sum;
    seq_t result = num / 4;               /* exact division, num is a multiple of 4 */
    return result;
}
