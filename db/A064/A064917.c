/*
 * A064917  a(n) is the result of beginning with n and iterating k ->
 *          A064916(k) until a prime is reached.
 * https://oeis.org/A064917
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is prime, else a(A064916(n)), where A064916(k) =
 *            k/lpf(k) + lpf(k) - 1 (iterate until a prime is reached)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 97 terms through n = 982 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A064917_step(uint64_t n)
{
    uint64_t p = 0;
    if (n % 2ULL == 0) {
        p = 2ULL;
    } else {
        for (uint64_t i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                p = i;
                break;
            }
        }
        if (p == 0)
            p = n;
    }
    return (n / p) + p - 1;
}

static bool A064917_isprime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    uint64_t k = (uint64_t)n;
    while (!A064917_isprime(k))
        k = A064917_step(k);
    return (seq_t)k;
}
