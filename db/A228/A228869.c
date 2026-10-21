/*
 * A228869  Numbers k such that 2 * (1^k + 2^k + 3^k + ... + k^k) == 0 (mod
 *          k).
 * https://oeis.org/A228869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer k such that 2 * (1^k + 2^k + …
 *            + k^k) == 0 (mod k). For odd k the condition always holds. For
 *            even k the condition holds iff for every odd prime p dividing k
 *            (p‑1) ∤ k. This is implemented by checking the prime factors of
 *            k and testing the divisibility rule
 * Range:     u8 n <= 204, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef MAX_PRIME
/* enough for k up to many thousands – exceeds √(max possible k) for the required limits */
#define MAX_PRIME 200000
#endif

static bool _is_prime[MAX_PRIME + 1];
static uint32_t _prime_list[MAX_PRIME / 2];
static size_t _prime_cnt = 0;
static bool _primes_computed = false;

static void _compute_primes(void) {
    for (int i = 0; i <= MAX_PRIME; i++) _is_prime[i] = true;
    _is_prime[0] = _is_prime[1] = false;
    for (int i = 2; (int64_t)i * i <= MAX_PRIME; ++i) {
        if (_is_prime[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i) _is_prime[j] = false;
        }
    }
    for (int i = 3; i <= MAX_PRIME; i += 2) {
        if (_is_prime[i]) _prime_list[_prime_cnt++] = (uint32_t)i;
    }
    _primes_computed = true;
}

/* returns true iff even k satisfies the OEIS condition */
static bool _even_ok(uint64_t k) {
    uint64_t orig = k;
    /* remove all factors 2 */
    while ((k & 1ULL) == 0) k >>= 1;

    uint64_t rem = k;
    for (size_t idx = 0; idx < _prime_cnt; ++idx) {
        uint32_t p = _prime_list[idx];
        uint64_t pp = (uint64_t)p * p;
        if (pp > rem) break;
        if (rem % p == 0) {
            /* p is an odd prime divisor of orig */
            if (orig % (p - 1) == 0) return false;
            while (rem % p == 0) rem /= p;
        }
    }
    if (rem > 1) { /* rem is an odd prime factor > sqrt(original odd part) */
        if (orig % (rem - 1) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (!_primes_computed) _compute_primes();

    idx_t count = 0;
    uint64_t k = 1;
    for (;;) {
        if ((k & 1ULL) || _even_ok(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
