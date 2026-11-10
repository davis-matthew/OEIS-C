/*
 * A070163  Primes arising in A070162.
 * https://oeis.org/A070163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A070162(n) - phi(A070162(n)) - 1, where A070162 lists the
 *            k with k - phi(k) - 1 prime; computed by an ascending search
 *            over k
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9982 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A070163_phi(uint64_t n)
{
    uint64_t result = n, t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result -= result / p;
        }
    }
    if (t > 1) result -= result / t;
    return result;
}

static bool A070163_isprime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t p = 5; p * p <= n; p += 6) {
        if (n % p == 0 || n % (p + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t found = 0;
    uint64_t k = 1, val = 0;
    while (found < n) {
        ++k;
        val = k - A070163_phi(k) - 1;
        if (A070163_isprime(val)) ++found;
    }
    return (seq_t)val;
}
