/*
 * A014689  a(n) = prime(n)-n, the number of nonprimes less than prime(n).
 * https://oeis.org/A014689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = prime(n) - n
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit numbers */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2ULL == 0ULL) return x == 2ULL;
    for (uint64_t i = 3ULL; i <= x / i; i += 2ULL) {
        if (x % i == 0ULL) return false;
    }
    return true;
}

/* Cached list of primes */
static uint64_t *g_primes = NULL;
static size_t    g_prime_cnt = 0;
static size_t    g_prime_cap = 0;
static uint64_t  g_last_checked = 1ULL;

/* Ensure at least n primes are cached.  Caller guarantees n>=1. */
static void ensure_primes(idx_t n)
{
    if (g_prime_cnt >= (size_t)n) return;

    if (!g_primes) {
        g_prime_cap = 128;
        g_primes = (uint64_t*)malloc(g_prime_cap * sizeof(uint64_t));
    }

    for (;;) {
        ++g_last_checked;
        if (is_prime_uint64(g_last_checked)) {
            if (g_prime_cnt >= g_prime_cap) {
                g_prime_cap <<= 1;
                g_primes = (uint64_t*)realloc(g_primes, g_prime_cap * sizeof(uint64_t));
            }
            g_primes[g_prime_cnt++] = g_last_checked;
            if (g_prime_cnt >= (size_t)n) break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for offset – not used in OEIS */
    ensure_primes(n);
    uint64_t prime_val = g_primes[n - 1];
    seq_t result = (seq_t)(prime_val - n);
    return result;
}
