/*
 * A072627  Number of divisors d of n such that d-1 is prime.
 * https://oeis.org/A072627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that (d‑1) is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Simple primality test for 64‑bit numbers (sufficient for n ≤ 10⁴). */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (uint64_t i = 3; i * i <= v; i += 2)
        if (v % i == 0) return false;
    return true;
}

/* Factorises n, storing distinct prime factors in `pr`, their
   exponents in `ex`, and the number of factors in `cnt`.          */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    while (n % 2 == 0) {
        if (c == 0 || pr[c - 1] != 2) { pr[c] = 2; ex[c] = 1; c++; }
        else ex[c - 1]++;          /* already recorded 2 */
        n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            size_t e = 0;
            while (n % p == 0) { n /= p; e++; }
            pr[c] = p; ex[c] = e; c++;
        }
    }
    if (n > 1) { pr[c] = n; ex[c] = 1; c++; }
    *cnt = c;
}

/* Recursively enumerates all divisors of n, counting those whose
   (divisor‑1) is prime.                                                       */
static void dfs_divisors(const uint64_t *pr, const uint64_t *ex,
                         size_t factor_cnt, size_t idx,
                         uint64_t current, size_t *counter)
{
    if (idx == factor_cnt) {
        if (current > 1 && is_prime_uint64(current - 1))
            (*counter)++;
        return;
    }

    uint64_t val = 1;
    for (uint64_t e = 0; e <= ex[idx]; e++) {
        dfs_divisors(pr, ex, factor_cnt, idx + 1, current * val, counter);
        val *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    uint64_t un = (uint64_t)n;

    uint64_t primes[64];
    uint64_t exponents[64];
    size_t factor_cnt = 0;
    factor_uint64(un, primes, exponents, &factor_cnt);

    size_t count = 0;
    dfs_divisors(primes, exponents, factor_cnt, 0, 1ULL, &count);

    return (seq_t)count;
}
