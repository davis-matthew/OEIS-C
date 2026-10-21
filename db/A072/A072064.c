/*
 * A072064  Least k>0 such that prime(n)+k*n is prime.
 * https://oeis.org/A072064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 3133, u32 n <= 3133, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A072064: a(n) = least k>0 such that prime(n)+k*n is prime.  */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;     /* Cache for already computed values. */
static size_t cache_cap = 0;    /* Current capacity of the cache buffer. */

/*  Return the least k>0 with prime(n)+k*n prime.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* offset 1 – undefined for n=0 */

    /* Ensure the cache is big enough. */
    if ((size_t)n > cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – fall back to direct calculation */
            goto compute;
        }
        cache = tmp;
        cache_cap = new_cap;
        /* Zero any newly allocated entries. */
        if (new_cap > old_cap)
            memset(cache + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
    }

    /* If we have this value already, return it. */
    if (cache[n - 1] != 0)
        return cache[n - 1];

compute:
    {
        seq_t p   = get_prime(n);        /* prime(n)          */
        seq_t step = (seq_t)n;           /* n                 */
        seq_t cand = p + step;           /* k = 1 candidate  */
        idx_t k = 1;

        while (!is_prime_big(cand)) {
            cand += step;
            ++k;
        }

        /* Store the result in cache if possible. */
        if ((size_t)n <= cache_cap)
            cache[n - 1] = (seq_t)k;

        return (seq_t)k;
    }
}
