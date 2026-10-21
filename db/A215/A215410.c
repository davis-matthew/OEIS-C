/*
 * A215410  a(0) = 0; a(n+1) = 2*a(n) + k where k = 0 if prime(n+2)/prime(n+1)
 *          < prime(n+1)/prime(n), otherwise k = 1.
 * https://oeis.org/A215410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215410:
   a(0) = 0
   a(n+1) = 2*a(n) + k,  n ≥ 0
   k = 0 if prime(n+2)/prime(n+1) < prime(n+1)/prime(n)
       1 otherwise
   (prime(1) = 2) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*  Prime generation – builds enough primes for the given n            */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;   /* 0‑based: prime_list[0] = 2 */
static size_t   prime_cnt  = 0;

/* obtain at least `needed` primes */
static void ensure_primes(size_t needed)
{
    if (prime_cnt >= needed)
        return;

    /* estimate an upper bound for the `needed`‑th prime */
    uint64_t limit;
    if (needed < 6) {
        limit = 15;
    } else {
        /* upper bound: p_n ≤ n (log n + log log n) for n ≥ 6 */
        double n = (double)needed;
        double est = n * (log(n) + log(log(n))) + 3.0; /* safety margin */
        limit = (uint64_t)est;
    }

    /* simple sieve – array of uint8_t for booleans */
    uint8_t *sieve = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!sieve) return;                    /* OOM – unlikely in tests */

    for (uint64_t i = 2; i <= limit; ++i)
        sieve[i] = 1;

    for (uint64_t p = 2; p * p <= limit; ++p)
        if (sieve[p])
            for (uint64_t m = p * p; m <= limit; m += p)
                sieve[m] = 0;

    /* allocate memory for primes and fill */
    uint64_t *new_list = (uint64_t *)realloc(prime_list, needed * sizeof(uint64_t));
    if (!new_list) { free(sieve); return; }
    prime_list = new_list;

    size_t count = 0;
    for (uint64_t i = 2; i <= limit && count < needed; ++i)
        if (sieve[i])
            prime_list[count++] = i;

    prime_cnt = needed;
    free(sieve);
}

/* ------------------------------------------------------------------ */
/*  Sequence cache                                                    */
/* ------------------------------------------------------------------ */
static seq_t *a_cache   = NULL;   /* 0‑based cache of a(n) values      */
static size_t cache_cap = 0;        /* allocated capacity                */
static size_t cache_len = 0;        /* number of terms computed so far    */

static void ensure_cache_size(size_t needed)
{
    if (cache_cap >= needed)
        return;

    seq_t *tmp = (seq_t *)realloc(a_cache, needed * sizeof(seq_t));
    if (!tmp) return;               /* OOM – unlikely in tests */
    a_cache = tmp;
    cache_cap = needed;
}

/* ------------------------------------------------------------------ */
/*  Main function                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;

    /* primes up to index n+2 (1‑based) are required */
    ensure_primes(target + 2);
    ensure_cache_size(target + 1);

    /* initialise first term if necessary */
    if (cache_len == 0) {
        a_cache[0] = 0;
        cache_len  = 1;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i <= target; ++i) {
        /* i: index of term being computed (0‑based)
           prime indices (0‑based in prime_list) */
        seq_t p_prev = (seq_t)prime_list[i - 1];     /* prime(i)   */
        seq_t p_curr = (seq_t)prime_list[i];        /* prime(i+1) */
        seq_t p_next = (seq_t)prime_list[i + 1];    /* prime(i+2) */

        bool less = (p_next * p_prev) < (p_curr * p_curr);
        seq_t k   = less ? 0 : 1;
        a_cache[i] = (a_cache[i - 1] << 1) + k;
    }

    cache_len = target + 1;
    return a_cache[target];
}
