/*
 * A088860  Twice the primorials (first definition), 2*A002110(n).
 * https://oeis.org/A088860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ∏_{i=1..n} p_i (p_i = i‑th prime)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) = 4) */

#include <stdlib.h>
#include <stdbool.h>

/* Cache of primes */
static uint64_t *prime_buf = NULL;    /* array of primes */
static size_t    prime_cnt = 0;       /* number of primes stored */
static size_t    prime_cap = 0;       /* allocated capacity */

/* Cache of sequence values */
static seq_t *cache_buf = NULL;      /* array of a(n) values */
static size_t  cache_sz  = 0;        /* number of values stored */
static size_t  cache_cap = 0;        /* allocated capacity */

/* Ensure at least 'n' primes are known.  */
static void
ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    /* Grow the prime array if needed */
    if (prime_cap < prime_cnt + (n - prime_cnt))
    {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        uint64_t *tmp = (uint64_t *)realloc(prime_buf, new_cap * sizeof(uint64_t));
        if (!tmp) abort();           /* allocation failure */
        prime_buf = tmp;
        prime_cap = new_cap;
    }

    /* Generate primes until we have n of them */
    uint64_t candidate = prime_cnt == 0 ? 2 : prime_buf[prime_cnt - 1] + 1;
    if (candidate % 2 == 0 && candidate != 2) candidate++;  /* ensure odd after 2 */

    while (prime_cnt < (size_t)n)
    {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = prime_buf[i];
            if (p * p > candidate) break;
            if (candidate % p == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            prime_buf[prime_cnt++] = candidate;
        }
        candidate += 2;   /* only odd candidates (2 already handled) */
    }
}

/* Ensure the sequence cache holds a(n) for all n up to 'n'. */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_sz) return;

    /* Grow cache array if needed */
    if (cache_cap < (size_t)n)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache_buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache_buf = tmp;
        cache_cap = new_cap;
    }

    /* Incremental computation of primorial values */
    seq_t running = 1;                /* product of primes up to cache_sz-1 */
    if (cache_sz > 0)
    {
        /* running = product of first cache_sz primes */
        running = cache_buf[cache_sz - 1] / 2;
    }

    for (size_t i = cache_sz; i < (size_t)n; ++i)
    {
        running *= (seq_t)prime_buf[i];
        cache_buf[i] = running * 2;   /* a(i+1) = 2 * product_{k=0}^{i} p_k */
    }
    cache_sz = (size_t)n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1 */
    if (n == 0) return 0;   /* arbitrary for out‑of‑range, not used */

    ensure_primes(n);
    ensure_cache(n);

    return cache_buf[(size_t)n - 1];
}
