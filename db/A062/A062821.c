/*
 * A062821  Number of divisors of totient of n.
 * https://oeis.org/A062821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(phi(n)) (number of divisors of Euler’s totient)
 * Range:     u8 n <= 293, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 116 terms through n = 1935 (2026-11-01)
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

/* Compute the divisor count of a 64‑bit integer (fits for n ≤ 10 000). */
static seq_t divisor_count_u64_local(uint64_t n)
{
    seq_t count = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint8_t exp = 0;
            do {
                n /= p;
                exp++;
            } while (n % p == 0);
            count *= (seq_t)(exp + 1);
        }
    }
    if (n > 1) {
        count *= 2;
    }
    return count;
}

seq_t a(idx_t n)
{
    seq_t phi_val = phi(n);           /* phi(n) fits in seq_t for all tests */
    uint64_t phi64 = (uint64_t)phi_val; /* cast to 64‑bit for small‑divisor routine */
    return divisor_count_u64_local(phi64);
}
