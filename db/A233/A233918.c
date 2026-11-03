/*
 * A233918  a(n) = |{0 < k <= n/2: (phi(k) + phi(n-k))/2 is prime}|, where
 *          phi(.) is Euler's totient function.
 * https://oeis.org/A233918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k <= n/2 : (phi(k) + phi(n-k)) / 2 is prime}|
 * Range:     u8 n <= 819, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9557 (2026-11-01)
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

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t kmax = n >> 1;           /* n / 2 */

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t phik = phi(k);
        seq_t phin = phi(n - k);
        seq_t sum  = phik + phin;

        /* Count only when the sum is even (so the average is an integer) */
        if ((sum & 1) == 0) {
            seq_t mid = sum / 2;
            if (is_prime((idx_t)mid))
                ++count;
        }
    }

    return count;
}
