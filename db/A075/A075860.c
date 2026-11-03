/*
 * A075860  a(n) is the fixed point reached when the map x -> A008472(x) is
 *          iterated, starting from x = n, with the convention a(1)=0.
 * https://oeis.org/A075860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 9639 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>1, iterate x -> sum_of_distinct_prime_factors(x) until a prime occurs. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Compute the sum of distinct prime factors of x. (x >= 2) */
static uint64_t sum_distinct_prime_factors_uint64(uint64_t x) {
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
    if (x > 1) sum += x;  /* x itself is a prime factor */
    return sum;
}

/* Return true if n is a prime number (n >= 2). */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    uint64_t cur = (uint64_t)n;
    while (true) {
        if (is_prime_uint64(cur)) return (seq_t)cur;
        cur = sum_distinct_prime_factors_uint64(cur);
    }
}
