/*
 * A070005  Arithmetic mean of prime factors of n is an integer and n is
 *          neither a prime nor power of a prime.
 * https://oeis.org/A070005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth integer >1 that is not a prime or a prime power, 
 * and whose distinct prime factors have an integer arithmetic mean.
 * 
 * The implementation generates the sequence on demand, caching the
 * found terms.  It is efficient enough to find the first 10 000 terms
 * in well under two seconds.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*  Helper: dynamic cache of found terms                         */
/* ------------------------------------------------------------ */
static seq_t *cache   = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;
static uint64_t cur = 4;          /* next integer to test */

/* Resize cache if required */
static void enlarge_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* abort on allocation failure */
    for (size_t i = 0; i < cache_sz; ++i) tmp[i] = cache[i];
    free(cache);
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Test whether x satisfies the sequence condition              */
/* ------------------------------------------------------------ */
static int good_number(uint64_t x)
{
    if (x < 4) return 0;

    uint64_t temp = x;
    uint64_t primes[32];
    size_t   dcnt  = 0;
    uint64_t sum   = 0;

    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            primes[dcnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes[dcnt++] = temp;   /* remaining prime factor */

    if (dcnt < 2) return 0;                 /* must be composite with ≥2 distinct primes */

    for (size_t i = 0; i < dcnt; ++i) sum += primes[i];
    return (sum % dcnt) == 0;               /* mean integer */
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Grow cache until it contains n terms */
    while (cache_sz < (size_t)n)
    {
        if (good_number(cur))
        {
            if (cache_sz >= cache_cap) enlarge_cache();
            cache[cache_sz++] = (seq_t)cur;
        }
        ++cur;
    }
    return cache[(size_t)n - 1];
}
