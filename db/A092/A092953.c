/*
 * A092953  Number of primes of the form n+p, where p is a prime < n.
 * https://oeis.org/A092953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < n such that n + p is prime
 * Range:     u8 n <= 3569, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/*  Static storage for primes, sieve limit, and cached a(n) values  */
static idx_t sieve_limit = 0;            /* maximum number currently sieved      */
static unsigned char *is_prime = NULL;   /* 0/1 flags for primality             */
static seq_t *cache = NULL;              /* cached a(n) values, indexed by n    */
static size_t cache_alloced = 0;         /* allocated bytes for cache array     */
static idx_t cache_max = 0;              /* highest n for which cache[n] is valid */

/*  Ensure that primes are known up to 'limit'    */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /*  Allocate new sieve array                                   */
    size_t sz = (size_t)(limit + 1);
    unsigned char *new_flag = (unsigned char*)malloc(sz);
    if (!new_flag) abort();
    memset(new_flag, 1, sz);
    new_flag[0] = new_flag[1] = 0;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (new_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_flag[j] = 0;
        }
    }

    free(is_prime);
    is_prime = new_flag;
    sieve_limit = limit;
}

/*  Ensure that the cache can hold values for all n <= 'n'   */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_alloced)
        return;
    size_t new_cap = ((size_t)n + 1) * 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    /*  Initialise new entries to 0                         */
    if (cache)
        memset((void*)(new_cache + cache_alloced), 0,
               (new_cap - cache_alloced) * sizeof(seq_t));
    cache = new_cache;
    cache_alloced = new_cap;
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    if (n == 0)  /* not used, but safe */
        return 0;

    /*  The largest value we need to prime-test is 2*n-1   */
    ensure_sieve(2 * n - 1);

    /*  Cache results up to n  */
    ensure_cache_capacity(n);
    if (n <= cache_max)
        return cache[n];

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t cnt = 0;
        for (idx_t p = 2; p < k; ++p) {
            if (is_prime[p] && is_prime[k + p])
                ++cnt;
        }
        cache[k] = cnt;
    }
    cache_max = n;
    return cache[n];
}
