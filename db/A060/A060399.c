/*
 * A060399  In base ten, we try to get the digits 1,2,3,...,9,0,1,2,3,... in
 *          order only using a sequence of increasing primes and each time
 *          choosing the smallest prime that will give the desired digit.
 * https://oeis.org/A060399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   containsDigit, isPrimeUInt64, nextPrimeUInt64_gt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060399
   a(1) = 11;
   For n>1, a(n) is the smallest prime > a(n-1) whose decimal representation
   contains the digit d(n) where d(n) cycles 1,2,3,4,5,6,7,8,9,0,1,2,...
   The function caches already computed terms so any order of calls is fast.
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/containsDigit.h"
#include "../utils/isPrimeUInt64.h"
#include "../utils/nextPrimeUInt64_gt.h"

/* ---------- cache for computed sequence values ---------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t lastPrime = 0;

/* ---------- grow cache if necessary ---------- */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    while (newcap < needed) newcap *= 2;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = newptr;
    cache_cap = newcap;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* invalid index guard */
    if (cache_size >= (size_t)n)              /* already computed */
        return cache[n - 1];

    /* compute missing terms up to n */
    for (idx_t cur = cache_size + 1; cur <= n; ++cur)
    {
        int d = ((cur - 1) % 10) + 1;
        if (d == 10) d = 0;                   /* cycle 1..9,0 */

        /* find next prime containing digit d */
        uint64_t cand = nextPrimeUInt64_gt(lastPrime);
        while (!containsDigit(cand, d))
            cand = nextPrimeUInt64_gt(cand);

        /* store result in cache */
        ensure_cache((size_t)cache_size + 1);
        cache[cache_size++] = (seq_t)cand;
        lastPrime = cand;
    }

    return cache[n - 1];
}
