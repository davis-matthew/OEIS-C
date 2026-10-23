/*
 * A014117  Numbers k such that m^(k+1) == m (mod k) holds for all m.
 * https://oeis.org/A014117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A014117: Numbers k such that m^(k+1) ≡ m (mod k) for all m.
   The sequence starts 1, 2, 6, 42, 1806, … .  It is generated as
   follows:

   a(1) = 1
   For n > 1:
     let p be the smallest prime > a(n-1) for which (p‑1) divides a(n-1);
     if such a prime exists set a(n) = a(n-1) * p,
     otherwise the sequence stabilises and a(n) = a(n-1) for all larger n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* small prime test, fast enough for primes up to a few million */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static state so that out‑of‑order calls are handled */
    static seq_t *cache = NULL;          /* cached terms */
    static size_t cache_cap = 0;          /* allocated capacity  */
    static idx_t cached_n = 0;            /* number of terms currently cached  */
    static uint64_t last_prime = 1;       /* last prime tried */
    static bool fixed = false;            /* true once sequence stops growing */

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation error; return 0 */
        cache[0] = (seq_t)1;                  /* a(1) = 1 */
        cached_n = 1;
    }

    /* generate terms up to requested index */
    while (cached_n < n) {
        if (fixed) {                      /* no further growth */
            cache[cached_n] = cache[cached_n - 1];
            cached_n++;
            continue;
        }

        /* find the next suitable prime */
        uint64_t candidate = last_prime + 1;
        bool found = false;
        /* safe upper bound on search attempts – enough for small n */
        const uint64_t SEARCH_LIMIT = 100000;
        for (uint64_t attempts = 0; attempts < SEARCH_LIMIT; ++candidate, ++attempts) {
            if (!is_prime_uint64(candidate)) continue;
            /* check (p-1) divides current product */
            seq_t remainder = cache[cached_n - 1] % (seq_t)(candidate - 1);
            if (remainder == 0) {
                found = true;
                break;
            }
        }

        if (!found) {                      /* no prime found – stabilize */
            fixed = true;
            cache[cached_n] = cache[cached_n - 1];
            cached_n++;
            continue;
        }

        /* grow cache if required */
        if (cached_n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL) return (seq_t)0; /* allocation error */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* compute next term */
        seq_t next = cache[cached_n - 1] * (seq_t)candidate;
        cache[cached_n] = next;
        cached_n++;
        last_prime = candidate;
    }

    return cache[n - 1];
}
