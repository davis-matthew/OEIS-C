/*
 * A176114  Numbers n such that 30*n-1, 30*n+1 are twin primes.
 * https://oeis.org/A176114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer N such that both 30*N‑1 and 30*N+1 are
 *            prime (i.e., N is a member of sequence A176114). The code
 *            searches sequentially for the required N
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cache to store the sequence values that have been found already. */
static idx_t *cache      = NULL; /* array of found n values */
static size_t cache_cap  = 0;    /* allocated capacity */
static size_t cache_len  = 0;    /* number of stored values */
static idx_t next_candidate = 1; /* next n to test */

/* Ensure the cache has space for at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap << 1 : 2;
    while (newcap < needed) newcap <<= 1;
    idx_t *tmp = (idx_t *)realloc(cache, newcap * sizeof(idx_t));
    cache = tmp;
    cache_cap = newcap;
}

/* Return the n-th element of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* undefined for n = 0; return 0 */

    while (cache_len < n) {
        idx_t p1 = 30ULL * next_candidate - 1;
        idx_t p2 = 30ULL * next_candidate + 1;
        if (is_prime(p1) && is_prime(p2)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return (seq_t)cache[n - 1];
}
