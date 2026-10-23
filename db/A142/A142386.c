/*
 * A142386  Primes congruent to 35 mod 47.
 * https://oeis.org/A142386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 35 (mod 47). Computed by
 *            incrementing candidates of the form 35 + 47k and testing each
 *            for primality. The first term is 223 (the first prime ≡ 35 mod
 *            47)
 * Range:     u8 n <= 1, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe return */

    /* static cache of found terms */
    static seq_t *cache = NULL;          /* array of a(i) for i ≥ 1 */
    static size_t cache_cap = 0;         /* capacity of cache */
    static size_t cache_cnt = 0;         /* number of terms stored */
    static idx_t cand = 223;             /* next candidate ≡ 35 mod 47 */

    /* extend cache until we have at least n terms */
    while (cache_cnt < n)
    {
        if (is_prime(cand))              /* check primality of candidate */
        {
            /* grow cache if needed */
            if (cache_cnt >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();  /* out‑of‑memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)cand;   /* store found prime */
        }
        cand += 47;                     /* next number with the same residue */
    }

    return cache[n - 1];                /* zero‑based index into cache */
}
