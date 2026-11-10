/*
 * A082087  a(n) is the fixed point if function A008472 is iterated when
 *          started at initial value n!.
 * https://oeis.org/A082087
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A075860(n!); the sum of distinct primes of n! is the sum
 *            of primes <= n, then iterate x -> A008472(x) until a prime (the
 *            fixed point) is reached
 * Range:     u8 n <= 42, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 86 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool A082087_isprime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

static uint64_t A082087_sopf(uint64_t x) {
    uint64_t sum = 0;
    if ((x & 1ULL) == 0) {
        sum += 2;
        while ((x & 1ULL) == 0) x >>= 1;
    }
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) {
            sum += d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) sum += x;
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t cur = 0;
    for (uint64_t p = 2; p <= N; p++)
        if (A082087_isprime(p)) cur += p;
    while (!A082087_isprime(cur))
        cur = A082087_sopf(cur);
    return (seq_t)cur;
}
