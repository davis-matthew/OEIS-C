/*
 * A079887  Values of y-x where p runs through the primes of form 4k+1 and
 *          p=x^2+y^2, 0<=x<=y.
 * https://oeis.org/A079887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y - x, where p = x² + y² is the n‑th prime ≡ 1 (mod 4)
 *            and 0 ≤ x ≤ y
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the sequence on demand, caching results.  It works for
   any integer type seq_t (including arbitrary‑precision classes) and idx_t
   (uint64_t).  No fixed table of values is used. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- integer square root (uint64_t) ---------- */
static inline uint64_t uint64_sqrt(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high   = mid - 1;
    }
    return high;
}

/* ---------- find y - x for a prime p = x² + y² (0 ≤ x ≤ y) ---------- */
static seq_t diff_for_prime(uint64_t p)
{
    uint64_t lim = uint64_sqrt(p);
    for (uint64_t x = 0; x <= lim; ++x) {
        uint64_t xsq = x * x;
        uint64_t ysq = p - xsq;
        uint64_t y   = uint64_sqrt(ysq);
        if (y * y == ysq) {
            if (x <= y) return (seq_t)(y - x);
            return (seq_t)(x - y);   /* a safety fallback */
        }
    }
    return (seq_t)0;   /* should never happen for valid primes          */
}

/* ---------- primality testing using a growing prime list ---------- */
static uint64_t *prime_list   = NULL; /* primes found so far */
static size_t   prime_cap    = 0;
static size_t   prime_cnt    = 0;
static uint64_t next_candidate = 2;   /* next number to test for primality */

/* Test whether m is divisible by any prime already in the list. */
static bool is_prime_candidate(uint64_t m)
{
    if (m < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > m / p) break; /* p*p > m */
        if (m % p == 0) return false;
    }
    return true;
}

/* ---------- cache for sequence values ---------- */
static seq_t *cache      = NULL; /* a(1) stored at cache[0]                 */
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Ensure that cache contains at least n values (n >= 1). */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    while (cache_len < (size_t)n) {
        /* find next prime */
        while (!is_prime_candidate(next_candidate)) {
            next_candidate += (next_candidate == 2 ? 1 : 2); /* skip even >2 */
        }

        /* store the prime */
        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 16;
            uint64_t *tmp  = (uint64_t*)realloc(prime_list,
                                               new_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            prime_list = tmp;
            prime_cap  = new_cap;
        }
        prime_list[prime_cnt++] = next_candidate;

        /* if p ≡ 1 mod 4, compute term */
        if ((next_candidate & 3) == 1) {
            seq_t val = diff_for_prime(next_candidate);

            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache,
                                            new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        /* advance to next candidate */
        next_candidate += (next_candidate == 2 ? 1 : 2);
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* behavior below the offset is irrelevant */
    ensure_cache(n);
    return cache[n - 1];
}
