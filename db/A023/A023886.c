/*
 * A023886  Average of proper divisors excluding 1 is an integer.
 * https://oeis.org/A023886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023886: the nth integer n such that the average of all proper
   divisors of n excluding 1 is an integer.  In formulaic form:

   a(n) = the n-th integer m ≥ 2 for which
          (∑_{d|m, 1<d<m} d) mod (τ(m)−2) = 0
   where τ(m) is the number of divisors of m.
   The sequence starts: 4, 8, 9, 15, 18, 21, 25, 27, 33, 35, 39, … */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* ---------- Prime sieve ---------- */
static uint64_t *primes      = NULL;   /* primes array */
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;

static void ensure_primes_upto(idx_t limit)
{
    if (prime_limit >= limit)
        return;
    free(primes);
    /* allocate enough space for all numbers up to limit */
    primes = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    prime_cnt = 0;
    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = (uint64_t)i;
            __uint128_t sq = (__uint128_t)i * i;
            if (sq <= limit) {
                for (idx_t j = (idx_t)sq; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    prime_limit = limit;
}

/* ---------- Integer square root (floor) ---------- */
static idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* ---------- Sequence cache ---------- */
#if defined(__cplusplus)
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* ---------- Main sequence routine ---------- */
idx_t   next_candidate = 2;   /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for index 0 – never used */
        return 0;

#if defined(__cplusplus)
    if (n <= cache.size())
        return cache[n - 1];
#else
    if (n <= cache_size)
        return cache[n - 1];
#endif

    for (;;) {
        idx_t m = next_candidate;
        ++next_candidate;          /* advance for next call */

        /* factorise m */
        idx_t temp = m;
        seq_t sigma = 1;   /* product of σ(pᵉ) */
        seq_t d     = 1;   /* product of (e+1) */

        /* run through primes up to sqrt(temp) */
        idx_t root = int_sqrt(temp);
        ensure_primes_upto(root);

        for (size_t i = 0; i < prime_cnt &&
             (__uint128_t)primes[i] * primes[i] <= temp; ++i) {

            uint64_t p = primes[i];
            if (temp % p != 0)
                continue;

            int exp = 0;
            seq_t p_pow = 1;
            seq_t part  = 1;            /* σ(pᵉ) */
            while (temp % p == 0) {
                temp /= p;
                exp++;
                p_pow = p_pow * (seq_t)p;
                part  = part + p_pow;
            }
            sigma = sigma * part;
            d     = d * (exp + 1);
        }

        if (temp > 1) {                 /* remaining prime factor */
            uint64_t p = temp;
            sigma = sigma * ((seq_t)1 + p);
            d     = d * 2;
        }

        seq_t sum_excl1   = sigma - (seq_t)m - 1;
        seq_t count_excl1 = d - 2;
        if (count_excl1 == 0)          /* primes – no proper divisors except 1 and itself */
            continue;

        if (sum_excl1 % count_excl1 == 0) {   /* average is integer */
#if defined(__cplusplus)
            cache.push_back((seq_t)m);
            if (cache.size() >= n)
                return cache[n - 1];
#else
            if (cache_size == cache_cap)
                grow_cache();
            cache[cache_size++] = (seq_t)m;
            if (cache_size >= n)
                return cache[n - 1];
#endif
        }
    }
}
