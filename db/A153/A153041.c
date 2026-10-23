/*
 * A153041  Numbers n >=10 such that 2*n-19 is not a prime.
 * https://oeis.org/A153041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are the integers n ≥ 10 such that 2*n − 19 is not prime; *
 *            i.e. a(1)=10, a(2)=14, a(3)=17,... (as listed in the OEIS entry)
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"      /* is_prime(idx_t x) – true if x is prime */

static seq_t *cache   = NULL;  /* holds a(1)..a(cache_len−1) */
static size_t  cache_cap = 0;  /* allocated capacity of cache  */
static size_t  cache_len = 0;  /* number of elements currently in cache   */
static idx_t   last_checked = 9;    /* last n that has been examined    */

/* Grow the cache if we need space for at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Returns the n‑th term of the sequence (n ≥ 1).  The function
 * generates terms on demand and remembers them so that subsequent
 * calls can be answered in constant time. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but safe to return 0 */

    while (cache_len < n) {
        idx_t curr = last_checked + 1;            /* candidate n   */
        /* 2*curr−19 may overflow only for astronomically large curr,
           but for the supported range (up to ~10⁴) it is safe. */
        idx_t test = 2 * curr - 19;
        if (!is_prime(test)) {                   /* 2*curr−19 not prime */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)curr;    /* store the term   */
        }
        last_checked = curr;                     /* update progress */
    }
    return cache[n - 1]; /* 0‑based index in cache */
}
