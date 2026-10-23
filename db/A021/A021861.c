/*
 * A021861  Decimal expansion of 1/857.
 * https://oeis.org/A021861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is the n‑th digit (0‑indexed) of the decimal expansion of 1/857. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cache of digits */
static idx_t cache_len = 0;   /* number of cached digits */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static uint64_t remainder = 1;/* current remainder for digit generation (initial 1) */
static const uint64_t DEN = 857ULL; /* divisor */

seq_t a(idx_t n)
{
    /* Ensure cache capacity for n+1 digits */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (n + 1 > cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Generate missing digits up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        uint64_t tmp = remainder * 10ULL;
        seq_t digit = (seq_t)(tmp / DEN);
        remainder = tmp % DEN;
        cache[i] = digit;
    }
    cache_len = n + 1;

    return cache[n];
}
