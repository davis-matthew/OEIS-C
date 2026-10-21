/*
 * A069187  Numbers k such that core(k) = ceiling(sqrt(k)) where core(k) is
 *          the squarefree part of k (the smallest integer such that k*core(k)
 *          is a square).
 * https://oeis.org/A069187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1. For n>=2, a(n) = m^2*(m^2+1) where m is the (n-1)th
 *            positive integer such that m^2+1 is squarefree
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 229, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


/* --------------------------------------------------------------------- */
/*  Helpers:  simple prime sieve up to 200000                            */
/* --------------------------------------------------------------------- */
#define PRIME_LIMIT 200000
static uint32_t primes[21000];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    bool *is_prime = (bool*)malloc(PRIME_LIMIT + 1);
    memset(is_prime, 1, PRIME_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
    {
        if (is_prime[i])
        {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (is_prime[i])
            primes[primes_cnt++] = (uint32_t)i;
    }
    free(is_prime);
}

/* --------------------------------------------------------------------- */
/*  Squarefree test for 64‑bit numbers                                    */
/* --------------------------------------------------------------------- */
static bool is_squarefree(uint64_t n)
{
    if (n < 2) return true;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint32_t p = primes[i];
        uint64_t psq = (uint64_t)p * (uint64_t)p;
        if (psq > n) break;
        int cnt = 0;
        while (n % p == 0)
        {
            n /= p;
            ++cnt;
            if (cnt > 1) return false;  /* square factor found */
        }
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Sequence cache                                                        */
/* --------------------------------------------------------------------- */
static seq_t *cache          = NULL;   /* array of a(2), a(3), ... */
static size_t cache_cap      = 0;
static size_t cache_len      = 0;
static idx_t  next_m         = 1;      /* next m to test (starting from 1) */

static void ensure_cache(idx_t n)
{
    if (n <= 1) return;                     /* a(1) is handled separately */
    if (cache_len >= (size_t)(n - 1)) return;

    init_primes();                           /* prepare primes only once */

    while (cache_len < (size_t)(n - 1))
    {
        /* find next m such that m^2+1 is squarefree */
        while (true)
        {
            seq_t mm = (seq_t)next_m;
            seq_t val = mm * mm + 1;         /* m^2 + 1 */
            if (is_squarefree((uint64_t)val))
                break;
            ++next_m;
        }

        seq_t mm = (seq_t)next_m;
        seq_t m2 = mm * mm;                  /* m^2 */
        seq_t val = m2 + 1;                  /* m^2+1 */
        seq_t k   = m2 * val;                /* m^2 * (m^2+1) */

        /* grow cache if necessary */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – abort */ abort(); }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = k;
        ++next_m;
    }
}

/* --------------------------------------------------------------------- */
/*  Main function                                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    ensure_cache(n);
    return cache[n - 2];   /* cache[0] holds a(2) */
}
