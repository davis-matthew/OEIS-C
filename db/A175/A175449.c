/*
 * A175449  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=17.
 * https://oeis.org/A175449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>1, a(n) is the smallest prime greater than a(n-1)
 *            such that a(n)+a(n-1) is a multiple of 17. The function memoizes
 *            the sequence, so calls in any order remain efficient
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Memoization array for the sequence values. */
static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;  /* current allocated size */

/* Return the n-th element of the sequence (a(1)=2). */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined index – return 0 */
        return (seq_t)0;

    /* initialise cache with a(1)=2 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)2;
        cache_cap = 1;
    }

    /* Grow cache if needed, doubling size for efficiency */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;  /* double */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms up to n if not already cached */
    if (cache[0] == 0) {                 /* safety check */
        cache[0] = (seq_t)2;
    }
    for (idx_t i = 1; i < n; ++i) {      /* i is zero‑based index for a(i+1) */
        seq_t prev = cache[i-1];
        /* desired residue r such that (cand + prev) % 17 == 0 */
        const seq_t MOD17 = (seq_t)17;
        seq_t rem_prev = prev % MOD17;
        seq_t r = (MOD17 - rem_prev) % MOD17;

        /* start searching with the first integer > prev */
        seq_t cand = prev + (seq_t)1;
        /* skip to the next odd number (after the start) */
        if (cand % (seq_t)2 == (seq_t)0 && cand != (seq_t)2)
            cand += (seq_t)1;

        /* search until a prime with the required residue is found */
        while (true) {
            if ((cand % MOD17) == r && is_prime_big(cand))
                break;
            cand += (seq_t)1;
            /* jump over evens except for number 2 */
            if (cand % (seq_t)2 == (seq_t)0 && cand != (seq_t)2)
                cand += (seq_t)1;
        }
        cache[i] = cand;
    }

    return cache[n-1];
}
