/*
 * A139407  Numbers k such that 8*k+3 and 8*k+7 are primes.
 * https://oeis.org/A139407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that 8*k+3 and 8*k+7 are both
 *            prime
 * Range:     u8 n <= 15, u16 n <= 1212, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;      /* stored k values      */
static size_t cache_len = 0;     /* number of values    */
static size_t cache_cap = 0;     /* allocated capacity  */
static idx_t next_k = 1;         /* next candidate k    */

static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void cache_append(idx_t k)
{
    grow_cache(cache_len + 1);
    cache[cache_len++] = (seq_t)k;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n<1 – fine */
    while (cache_len < (size_t)n) {
        idx_t k = next_k++;                  /* next candidate   */
        if (is_prime(8 * k + 3) && is_prime(8 * k + 7))
            cache_append(k);
    }
    return cache[n - 1];
}
