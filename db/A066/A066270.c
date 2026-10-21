/*
 * A066270  Multiples of 24 whose digits also sum to 24.
 * https://oeis.org/A066270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer that is divisible by 24 and
 *            whose decimal digits sum to 24
 * Range:     u8 none, u16 n <= 440, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digitSum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digitSum.h"

/* Cached sequence */
static seq_t *cache = NULL;      /* array of found terms */
static idx_t  cached = 0;        /* number of terms cached */

/* Parameters for the search */
static const seq_t step = (seq_t)24;   /* multiples of 24 */
static idx_t curMultiple = 0;         /* index into multiples of 24 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1; return 0 for safety */

    /* Return cached value if already computed */
    if (n <= cached) return cache[n-1];

    /* Extend cache storage if needed */
    size_t newsize = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);      /* allocation failed */
    cache = tmp;

    /* Find terms until we have at least n of them */
    while (cached < n) {
        curMultiple++;                     /* next multiple of 24 */
        seq_t candidate = (seq_t)curMultiple * step;
        if (digitSum(candidate) == (seq_t)24)
            cache[cached++] = candidate;   /* store term */
    }

    return cache[n-1];
}
