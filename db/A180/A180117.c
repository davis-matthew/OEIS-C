/*
 * A180117  Numbers n such that n and n+2 are both divisible by exactly 3
 *          primes (counted with multiplicity).
 * https://oeis.org/A180117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A180117: Numbers n such that both n and n+2 have exactly 3 prime
 * factors (counted with multiplicity).  The function searches for the
 * nth such number by testing integers in increasing order and uses a
 * precomputed smallest‑prime‑factor array for fast bigomega queries.
 *
 * a(1) = 18, a(2) = 28, a(3) = 42, …
 */

#include <stdint.h>
#include <stdlib.h>

/* maximum value we pre‑compute primes for – enough for 10 000 terms
 * (empirically < 500 k). */
#define MAX_SIEVE 5000000

static uint32_t *spf = NULL;          /* smallest prime factor array   */
static size_t   spf_size = 0;         /* size of spf array             */

static void init_sieve(size_t limit)
{
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_size = limit + 1;
}

/* count bigomega (prime factors with multiplicity) for x <= MAX_SIEVE */
static int bigomega(uint64_t x)
{
    int cnt = 0;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        do { x /= p; ++cnt; } while (x % p == 0);
    }
    return cnt;
}

/* cache of already found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* entry point */
seq_t a(idx_t n)
{
    /* initialise once */
    static int initialised = 0;
    if (!initialised) {
        init_sieve(MAX_SIEVE);
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
        initialised = 1;
    }

    if (n == 0) return 0;                /* offset is 1 – unused case */

    if ((idx_t)n <= cache_size)          /* already computed */
        return cache[n - 1];

    /* start searching from the first number after the last cached one */
    idx_t candidate = 2;
    if (cache_size > 0)
        candidate = (idx_t)cache[cache_size - 1] + 1;

    while (cache_size < (size_t)n) {
        /* only test even candidates – because n and n+2 must both be even
         * if they each have an odd number of prime factors? No need, just
         * test all numbers. */
        if (bigomega(candidate) == 3 && bigomega(candidate + 2) == 3) {
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return cache[n - 1];
}
