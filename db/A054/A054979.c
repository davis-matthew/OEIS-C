/*
 * A054979  e-perfect numbers: numbers k such that the sum of the e-divisors
 *          (exponential divisors) of k equals 2*k.
 * https://oeis.org/A054979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 571, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for e‑perfect numbers by checking multiples of 36 */
/* An e‑perfect number is one for which the sum of all its exponential
   divisors equals twice the number itself.  This implementation
   builds the sequence on demand, caching previously found terms.  It
   works for any seq_t that supports +, *, and comparisons, and for
   n as a 64‑bit index.  For larger n the required integer size must
   accommodate the terms; factorisation is limited to 64‑bit values. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- factorise a uint64_t ------------------------------------------- */
static void factorize_u64(uint64_t n, uint64_t primes[64],
                          uint64_t exps[64], size_t *cnt)
{
    *cnt = 0;
    uint64_t e = 0;

    /* factor 2 */
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++e;
    }
    if (e) {
        primes[(*cnt)] = 2;
        exps[(*cnt)]   = e;
        ++(*cnt);
    }

    /* odd factors */
    for (uint64_t d = 3; d * d <= n; d += 2) {
        e = 0;
        while (n % d == 0) {
            n /= d;
            ++e;
        }
        if (e) {
            primes[(*cnt)] = d;
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
    }

    if (n > 1) {           /* remaining prime factor */
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* ----- test whether k is e‑perfect ----------------------------------- */
static bool is_e_perfect_uint64(uint64_t k)
{
    uint64_t primes[64], exps[64];
    size_t  cnt;

    factorize_u64(k, primes, exps, &cnt);

    unsigned __int128 prod   = 1;
    unsigned __int128 target = (unsigned __int128)k * 2ULL;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prime = primes[i];
        uint64_t expo  = exps[i];

        unsigned __int128 sum = 0;
        unsigned __int128 pow = 1;

        for (uint64_t d = 1; d <= expo; ++d) {
            pow *= prime;
            if (expo % d == 0)
                sum += pow;
        }
        prod *= sum;
        if (prod > target)
            return false;
    }
    return prod == target;
}

/* ----- cache for the sequence ---------------------------------------- */
static seq_t *cache     = NULL;   /* 0‑based array of terms found */
static size_t cache_len = 0;      /* number of stored terms */

/* Ensure the cache holds at least 'n' terms (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    size_t new_len = (size_t)n;
    void *tmp_ptr = realloc(cache, new_len * sizeof(seq_t));
    if (!tmp_ptr && new_len) {
        abort();                /* allocation failed */
    }
    cache = (seq_t *)tmp_ptr;   /* cast required for C++ */

    /* Generate terms until the cache is full */
    while (cache_len < new_len) {
        uint64_t candidate;
        if (cache_len == 0)
            candidate = 36ULL;               /* first multiple of 36 */
        else
            candidate = (uint64_t)cache[cache_len - 1] + 36ULL;

        /* Search for the next e‑perfect number */
        while (!is_e_perfect_uint64(candidate))
            candidate += 36ULL;

        cache[cache_len++] = (seq_t)candidate;
    }
}

/* ----- exported sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n <= 0 */
    ensure_cache(n);
    return cache[n - 1];
}
