/*
 * A084199  Smallest prime q < prime(n) such that q+1 divides prime(n)+1, a(n)
 *          = 1 if no such q exists.
 * https://oeis.org/A084199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 9982, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A084199
   Smallest prime q < prime(n) such that q+1 divides prime(n)+1,
   a(n) = 1 if no such q exists.  The sequence is computed by
   enumerating primes up to the n‑th and scanning all smaller primes
   for the congruence condition.  Results are cached for any order of
   calls. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- primality handling -------------------------------------------------- */

/* Dynamic array of primes (as uint64_t – all primes used fit in 64 bits). */
static uint64_t *primes = 0;
static size_t   primes_count = 0;
static idx_t   sieve_limit   = 0;

/* Ensure that at least 'need' primes are available in 'primes'. */
static void init_primes_until(size_t need)
{
    if (primes_count >= need) return;

    idx_t limit = sieve_limit ? sieve_limit : (idx_t)2;
    while (1)
    {
        bool *is_composite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
        if (!is_composite) return;        /* allocation failure – let caller handle */

        for (idx_t i = 2; i * i <= limit; ++i)
            if (!is_composite[i])
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;

        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (!is_composite[i]) ++cnt;

        if (cnt >= need)
        {
            uint64_t *new_arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
            if (!new_arr) { free(is_composite); return; }

            size_t idx = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (!is_composite[i]) new_arr[idx++] = i;

            free(primes);
            primes = new_arr;
            primes_count = cnt;
            sieve_limit   = limit;
            free(is_composite);
            return;
        }

        free(is_composite);
        limit <<= 1;                     /* double the range */
        if (limit == 0) limit = UINT64_MAX;
    }
}

/* --- memoisation of a(n) ----------------------------------------------- */

/* Cache of computed a(n) values; 0 (default-constructed) means "not yet set". */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(size_t n)
{
    if (cache_cap >= n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 2;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                  /* allocation failure – let caller handle */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* --- main sequence function --------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 1;              /* out of offset – harmless */

    /* Ensure cache slot exists. */
    ensure_cache((size_t)n);
    if (cache[n - 1] != 0) return cache[n - 1];  /* already computed */

    /* Generate enough primes. */
    init_primes_until((size_t)n);
    uint64_t p = primes[n - 1];           /* n-th prime */
    seq_t   p_plus = (seq_t)p + 1;         /* (prime(n)+1) */
    seq_t   ans = 1;                       /* default if no divisor found */

    /* Scan all smaller primes for the smallest q with q+1 | p+1. */
    for (size_t i = 0; i + 1 < (size_t)n; ++i)
    {
        seq_t q = (seq_t)primes[i];
        if (p_plus % (q + 1) == 0)
        {
            ans = q;
            break;
        }
    }

    cache[n - 1] = ans;
    return ans;
}
