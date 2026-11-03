/*
 * A233544  Number of ways to write n = k^2 + m with k > 0 and m >= k^2 such
 *          that sigma(k^2) + phi(m) is prime, where sigma(k^2) is the sum of
 *          all (positive) divisors of k^2, and phi(.) is Euler's totient
 *          function (A000010).
 * https://oeis.org/A233544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(k,m): k>0, m≥k², n = k² + m, and σ(k²)+φ(m) is prime}|
 * Range:     u8 n <= 881, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9744 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

/* Return true if x >= 2 is prime (deterministic trial division) */
static inline bool is_prime(idx_t x)
{
    if (x < 2)                         return false;
    if (x == 2 || x == 3)              return true;
    if (x % 2 == 0)                    return false;
    for (idx_t i = 3; i <= x / i; i += 2)
        if (x % i == 0)                return false;
    return true;
}

/* return the sum of the positive divisors of n */
static seq_t sigma_of_n(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += d;
            if (d != n / d)
                sum += n / d;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 1;; ++k) {
        idx_t k2 = k * k;
        if (k2 > n)          /* k² alone exceeds n */
            break;
        idx_t m = n - k2;
        if (m < k2)          /* further k will only make m smaller */
            break;
        seq_t sigma = sigma_of_n(k2);
        seq_t phi_m  = phi(m);
        seq_t tot    = sigma + phi_m;
        if (is_prime((idx_t)tot))
            ++cnt;
    }
    return cnt;
}
