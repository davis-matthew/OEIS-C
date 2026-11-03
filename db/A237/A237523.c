/*
 * A237523  a(n) = |{0 < k < n/2: phi(k*(n-k)) + 1 is a square}|, where phi(.)
 *          is Euler's totient function.
 * https://oeis.org/A237523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n/2 : phi(k*(n-k)) + 1 is a square}|
 * Range:     u8 n <= 23, u16 n <= 4071, u32 n <= 4071, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9615 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

static seq_t sqrt_floor(seq_t x)
{
    if (x < 2) return x;               /* 0 or 1 */
    seq_t low = 0, high = x;
    while (low <= high)
    {
        seq_t mid = (low + high) / 2;
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* The allowed k satisfy 0 < k < n/2.
       For integer arithmetic this is equivalent to k <= (n-1)/2. */
    idx_t lim = (n - 1) / 2;
    seq_t count = 0;

    for (idx_t k = 1; k <= lim; ++k)
    {
        idx_t prod = k * (n - k);          /* k*(n-k)  (fits in idx_t) */
        seq_t ph   = phi(prod);             /* Euler's totient */
        seq_t val  = ph + 1;                /* phi(k*(n-k)) + 1   */
        seq_t s    = sqrt_floor(val);       /* floor(sqrt(val))   */

        if (s * s == val)
            ++count;
    }

    return count;
}
