/*
 * A021872  Decimal expansion of 1/868.
 * https://oeis.org/A021872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (floor(10^n / 868)) mod 10 (digits of the decimal
 *            expansion of 1/868)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL;
static idx_t cache_len   = 0;    /* number of terms currently cached (next free index) */
static idx_t cache_cap   = 0;    /* allocated capacity of cache array */
static uint64_t rem      = 1;    /* remainder for the next digit (r_k) */

/* Compute the nth term a(n) of sequence A021872 */
seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache has enough capacity for indices up to n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms using the remainder method */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* digit = floor((rem * 10) / 868) */
        uint64_t tmp = rem * 10ULL;
        seq_t d = (seq_t)(tmp / 868ULL);

        /* store digit */
        cache[i] = d;

        /* update remainder for next iteration */
        rem = tmp % 868ULL;
    }

    cache_len = n + 1;
    return cache[n];
}
