/*
 * A141552  Transformed products of prime factors of the composites, the
 *          largest prime and smallest prime incremented by 1.
 * https://oeis.org/A141552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of prime factors of the n‑th composite, with one
 *            instance of the largest prime factor replaced by (pmax+1) and
 *            one instance of the smallest prime factor replaced by (pmin+1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The computation uses a simple on‑demand cache.  Primality testing
    and factorisation are performed by small trial‑division routines
    that work for the range of n considered in the tests.  */

/*  The code is valid C11 and C++17.  No I/O is performed.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ----- Utility functions – small trial division ***** */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; p += (p == 2 ? 1 : 2)) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* ----- Cache for computed terms ***** */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of computed terms */
static size_t cache_cap  = 0;   /* allocated capacity */
static uint64_t next_num = 3;   /* will become 4 on first use */

/* Ensure the cache contains at least n terms */
static void ensure_cache(size_t n)
{
    if (cache_len >= n) return;

    /* Grow the underlying array if necessary */
    if (cache_cap == 0 || cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Compute additional terms until cache_len == n */
    while (cache_len < n) {
        /* Find next composite number */
        do { ++next_num; } while (is_prime(next_num));
        /* Factor it */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(next_num, primes, exps, &cnt);

        /* Identify smallest and largest prime factors */
        uint64_t pmin = primes[0];
        uint64_t pmax = primes[0];
        for (size_t i = 1; i < cnt; ++i) {
            if (primes[i] < pmin) pmin = primes[i];
            if (primes[i] > pmax) pmax = primes[i];
        }

        /* Compute transformed product using seq_t arithmetic */
        seq_t val = (seq_t)next_num *
                    (seq_t)(pmax + 1) *
                    (seq_t)(pmin + 1) /
                    ((seq_t)pmax * (seq_t)pmin);

        cache[cache_len++] = val;
    }
}

/* ----- Public sequence function ***** */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n <= 0 */
    ensure_cache((size_t)n);
    return cache[(size_t)(n - 1)];
}
