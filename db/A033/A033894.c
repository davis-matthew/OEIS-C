/*
 * A033894  Sort then Add, a(1)=5.
 * https://oeis.org/A033894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 5; for n > 1, a(n) = a(n‑1) + sortedDigits(a(n‑1))
 *            sortedDigits(x) = integer obtained by sorting the decimal digits
 *            of x in ascending order
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   sorted_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sorted_digits.h"

/* Cache for previously computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_max = 0;    /* number of terms stored (a(1) is cache[0]) */

/* External function required by the OEIS framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset is 1; behaviour below that is irrelevant */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)5;  /* a(1) = 5 */
        cache_max = 1;
    }

    /* Extend the cache up to the requested index. */
    while (cache_max < n) {
        seq_t prev    = cache[cache_max - 1];
        seq_t added   = sorted_digits(prev);
        seq_t next    = prev + added;

        /* Grow the cache if necessary. */
        if (cache_max == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_max] = next;
        cache_max++;
    }

    return cache[n - 1];
}
