/*
 * A140857  Primes of the form 210k + 73.
 * https://oeis.org/A140857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 210 k + 73 (OEIS A140857) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* cache for already found primes */
static seq_t *cache      = NULL;   /* array of primes */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of primes stored */
static idx_t next_k       = 0;     /* next k to check */

/* enlarge cache to hold at least 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap < 8 ? 8 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 0 */
        return 0;

    while (cache_len < n) {
        seq_t cand = (seq_t)73 + (seq_t)210 * (seq_t)next_k;
        if (is_prime_big(cand))
            ensure_cache_capacity(cache_len + 1), cache[cache_len++] = cand;
        next_k++;
    }
    return cache[n - 1];
}
