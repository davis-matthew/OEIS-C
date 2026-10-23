/*
 * A142748  Primes congruent to 21 mod 59.
 * https://oeis.org/A142748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that is congruent to 21 modulo 59 (OEIS
 *            A142748)
 * Range:     u8 n <= 1, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

/* Dynamically grow an array of uint64_t primes. */
static void grow_array(size_t *cap, uint64_t **arr, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        while (newcap < needed) newcap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(*arr, newcap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = newcap;
    }
}

/* Miller‑Rabin primality test for 64‑bit integers. */
static uint64_t mod_pow_u64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t base   = a % mod;
    while (d) {
        if (d & 1ULL) result = mul_mod64(base, result, mod);
        base = mul_mod64(base, base, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* factor n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Global state for the prime list. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;
static uint64_t next_k    = 0;   /* next value of k in 21 + 59*k */

/* Make sure we have at least n primes in prime_list. */
static void ensure_primes_u64(idx_t n)
{
    while (prime_cnt < (size_t)n) {
        uint64_t cand = 21ULL + 59ULL * next_k++;
        if (is_prime_u64(cand)) {
            grow_array(&prime_cap, &prime_list, prime_cnt + 1);
            prime_list[prime_cnt++] = cand;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, harmless */
    ensure_primes_u64(n);
    return (seq_t)prime_list[n - 1];
}
