/*
 * A143741  Aliquot sequence starting at 72.
 * https://oeis.org/A143741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 72; a(n+1) = sum of proper divisors of a(n) (aliquot sum)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Validated: partial, pass, 102 terms through n = 101 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper: compute sum of proper divisors of an unsigned integer (seq_t is assumed
   to be an unsigned integer type that can hold the intermediate result). */
static seq_t proper_divisor_sum(seq_t x)
{
    /* For seq_t values that exceed 64 bits, this routine may overflow.
       The aliquot sequence starting from 72 stays within 128 bits
       for all tested terms, so a 64‑bit intermediate is safe. */
    uint64_t n = (uint64_t)x;
    if (n <= 1) return (seq_t)0;
    uint64_t sum = 1;
    for (uint64_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += i;
            uint64_t j = n / i;
            if (j != i) sum += j;
        }
    }
    return (seq_t)sum;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with initial term a(0) = 72 */
    if (cache == NULL) {
        cache_len = 1;
        cache = mem_new(seq_t, 1);
        if (cache == NULL) abort();        /* allocation failed */
        cache[0] = (seq_t)72;
    }

    /* extend cache up to requested index */
    while (cache_len <= (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        seq_t next = proper_divisor_sum(prev);
        seq_t *tmp = mem_resize(seq_t, cache, (cache_len + 1));
        if (tmp == NULL) abort();          /* allocation failed */
        cache = tmp;
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[(size_t)n];
}
