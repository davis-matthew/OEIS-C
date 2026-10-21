/*
 * A050328  Number of ordered factorizations of n into squarefree numbers > 1.
 * https://oeis.org/A050328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k|n, k squarefree, k≥2} a(n/k) with a(1)=1 (OEIS
 *            A050328: number of ordered factorizations of n into square‑free
 *            >1)
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* global cache for computed values */
static seq_t  *cache = NULL;          /* cache[1..N] holds a(n) values   */
static idx_t   cached_up_to = 0;      /* largest n with a(n) stored      */

/* smallest prime factor array kept up to the largest n needed */
static uint32_t *spf = NULL;          /* spf[i] = smallest prime dividing i */
static idx_t   spf_size = 0;          /* current size of spf array          */

/* ensure spf array is computed up to at least limit */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_size) return;

    /* allocate new array */
    uint32_t *new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();            /* out of memory */
    memset(new_spf, 0, (limit + 1) * sizeof(uint32_t));

    /* simple sieve of Eratosthenes for spf */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {          /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }
    new_spf[1] = 1;

    /* replace old array */
    free(spf);
    spf = new_spf;
    spf_size = limit;
}

/* ensure that cache has values up to at least n */
static void ensure_cache_up_to(idx_t n)
{
    if (n <= cached_up_to) return;     /* nothing to do */

    /* grow cache array */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* base case */
    if (cached_up_to == 0) {
        cache[1] = (seq_t)1;
        cached_up_to = 1;
    }

    /* compute a(m) for every m > cached_up_to up to n */
    for (idx_t m = cached_up_to + 1; m <= n; ++m) {
        /* factor m to get its distinct primes */
        uint32_t primes[64];
        size_t   pcnt   = 0;
        idx_t    x      = m;

        while (x > 1) {
            uint32_t p = spf[x];
            primes[pcnt++] = p;
            while (x % p == 0) x /= p;
        }

        /* iterate over all non‑empty subsets of the distinct primes
           every subset gives a square‑free divisor k (product of selected primes) */
        int subsets = 1 << (int)pcnt;          /* pcnt <= 5 for m <= 10000  */
        seq_t sum = 0;

        for (int mask = 1; mask < subsets; ++mask) {
            idx_t k = 1;
            for (int j = 0; j < (int)pcnt; ++j)
                if (mask & (1 << j))
                    k *= (idx_t)primes[j];

            idx_t m_div_k = m / k;
            sum += cache[m_div_k];
        }
        cache[m] = sum;
    }
    cached_up_to = n;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* first, ensure we can factor up to n */
    ensure_spf(n);

    /* now ensure cache has value for n */
    ensure_cache_up_to(n);

    return cache[n];
}
