/*
 * A186298  A007520(n)-2.
 * https://oeis.org/A186298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n, 8k+3) – 2 (n ≥ 1)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* global state */
static idx_t next_candidate = 3;

#ifdef __cplusplus
static std::vector<idx_t> primes;   /* all primes found so far          */
static std::vector<seq_t> cache;    /* a(k) values for k ≥ 1           */
#else
/* sufficient limits for the intended test range (n ≤ ~10 000) */
#define MAX_PRIMES 200000
#define MAX_CACHE  200000
static idx_t primes[MAX_PRIMES];
static seq_t cache[MAX_CACHE];
static size_t prime_len = 0;   /* number of primes in primes[] */
static size_t cache_len  = 0;   /* number of a(k) values stored  */
#endif

/* ensure 2 is present in the prime list */
static void init_primes(void)
{
#ifdef __cplusplus
    if (primes.empty())
        primes.push_back(2);
#else
    if (prime_len == 0)
        primes[prime_len++] = 2;
#endif
}

/* primality test using the primes discovered so far */
static int is_prime(idx_t num)
{
    if (num < 2)
        return 0;
#ifdef __cplusplus
    for (size_t i = 0; i < primes.size(); ++i)
    {
        idx_t p = primes[i];
        if (p > num / p)   /* p*p > num, stop */
            break;
        if (num % p == 0)
            return 0;
    }
#else
    for (size_t i = 0; i < prime_len; ++i)
    {
        idx_t p = primes[i];
        if (p > num / p)   /* p*p > num, stop */
            break;
        if (num % p == 0)
            return 0;
    }
#endif
    return 1;
}

/* grow the cache until it contains a(n) */
static void generate_up_to(idx_t n)
{
    init_primes();

    /* check current length */
#ifdef __cplusplus
    if (n <= cache.size())
        return;
#else
    if (n <= cache_len)
        return;
#endif

    while (
#ifdef __cplusplus
           cache.size() < n
#else
           cache_len < n
#endif
    )
    {
        if (is_prime(next_candidate))
        {
            /* store prime for future divisibility tests */
#ifdef __cplusplus
            primes.push_back(next_candidate);
#else
            if (prime_len < MAX_PRIMES)
                primes[prime_len++] = next_candidate;
#endif
            /* if prime ≡ 3 (mod 8), store a(k) = prime–2 */
            if (next_candidate % 8 == 3)
            {
#ifdef __cplusplus
                cache.push_back((seq_t)next_candidate - 2);
#else
                if (cache_len < MAX_CACHE)
                    cache[cache_len++] = (seq_t)next_candidate - 2;
#endif
            }
        }
        next_candidate += 2;   /* only odd candidates needed */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* under‑offset is ignored */
        return 0;
    generate_up_to(n);
    return
#ifdef __cplusplus
           cache[(size_t)(n - 1)];
#else
           cache[n - 1];
#endif
}
