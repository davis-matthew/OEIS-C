/*
 * A071260  Integer part of the geometric mean of the n-th prime prime(n) and
 *          the n-th composite number C(n).
 * https://oeis.org/A071260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(prime(n) * C(n)))
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/isPrimeGeneric.h"        /* bool isPrimeGeneric(uint64_t) */
#include "../utils/sieve_first_n_primes.h"  /* void sieve_first_n_primes(idx_t n,
                                              uint32_t **primes_out, size_t *cnt_out) */

/* Integer square root for 128‑bit numbers, used for the geometric mean */
static unsigned __int128 isqrt128(unsigned __int128 v)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64) - 1;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* Prime caching ----------------------------------------------------- */
static uint64_t *prime_cache      = NULL;
static size_t   prime_cache_len   = 0;

/* Ensure that at least `need` primes are cached (0‑based index).  */
static void ensure_primes(size_t need)
{
    if (prime_cache_len >= need)
        return;

    uint32_t *tmp32 = NULL;
    size_t    cnt   = 0;
    sieve_first_n_primes((idx_t)need, &tmp32, &cnt);

    uint64_t *tmp64 = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!tmp64)
        abort();   /* memory allocation failed */

    for (size_t i = 0; i < cnt; ++i)
        tmp64[i] = tmp32[i];

    free(tmp32);

    free(prime_cache);
    prime_cache      = tmp64;
    prime_cache_len  = cnt;
}

/* ------------------------------------------------------------------ */
/* Composite caching ------------------------------------------------- */
static uint64_t *comp_cache            = NULL;
static size_t   comp_cache_len         = 0;
static uint64_t comp_next_candidate    = 4;          /* first composite */

static void ensure_composites(size_t need)
{
    if (comp_cache_len >= need)
        return;

    uint64_t *new_arr = (uint64_t *)realloc(comp_cache,
                                            need * sizeof(uint64_t));
    if (!new_arr)
        abort();   /* memory allocation failed */

    size_t old = comp_cache_len;
    for (size_t i = old; i < need; ++i) {
        while (isPrimeGeneric(comp_next_candidate))
            ++comp_next_candidate;           /* skip primes */
        new_arr[i] = comp_next_candidate;
        ++comp_next_candidate;
    }
    comp_cache = new_arr;
    comp_cache_len = need;
}

/* ------------------------------------------------------------------ */
/* Main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    /* OEIS offset is 0; the first prime/composite needed is at index 0. */
    size_t need = (size_t)n + 1;          /* number of terms required */

    ensure_primes(need);
    ensure_composites(need);

    uint64_t prime = prime_cache[n];        /* 0‑based index */
    uint64_t comp  = comp_cache[n];

    unsigned __int128 prod    = (unsigned __int128)prime *
                                (unsigned __int128)comp;
    unsigned __int128 sqrt128 = isqrt128(prod);

    return (seq_t)sqrt128;
}
