/*
 * A177833  Numbers k such that k^2 - 13 and k^2 + 13 are primes.
 * https://oeis.org/A177833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 378, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th integer k such that k^2 – 13 and k^2 + 13 are primes.
   a(1) = 4.  Cache results for subsequent calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Trial division by small primes to speed up */
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    static const size_t small_cnt = sizeof(small_primes)/sizeof(small_primes[0]);

    for (size_t i = 0; i < small_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Express n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    /* Bases that are deterministic for 64‑bit numbers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    static const size_t base_cnt = sizeof(bases)/sizeof(bases[0]);

    for (size_t i = 0; i < base_cnt; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;   /* base equal to n is trivial */
        /* Compute a^d mod n using binary exponentiation */
        __uint128_t res = 1;
        __uint128_t base = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1ULL)
                res = (res * base) % n;
            base = (base * base) % n;
            exp >>= 1ULL;
        }
        if (res == 1 || res == n - 1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            res = (res * res) % n;
            if (res == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside OEIS offset, undefined */

    /* ---- Static cache for already found terms ---- */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static uint64_t next_k = 4;          /* start from first known term */

    /* Grow cache if we need more entries */
    if (cache_size < (size_t)n) {
        if (cache_cap == 0) cache_cap = 4;
        while (cache_cap < (size_t)n) cache_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;

        /* Find new terms until cache has n entries */
        while (cache_size < (size_t)n) {
            uint64_t k = next_k++;
            /* Compute k^2 ± 13 safely using 128‑bit arithmetic */
            __uint128_t sq = (__uint128_t)k * k;
            /* sq is guaranteed < 2^64 for the intended range of n */
            uint64_t val1 = (uint64_t)(sq - 13);
            uint64_t val2 = (uint64_t)(sq + 13);

            if (is_prime_uint64(val1) && is_prime_uint64(val2)) {
                cache[cache_size++] = (seq_t)k;
            }
        }
    }

    return cache[(size_t)n - 1];
}
