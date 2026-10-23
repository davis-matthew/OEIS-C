/*
 * A185309  a(0)=0, a(1)=0; for n>1, a(n) = a(n-1) + (n+1)*a(n-2) + 1.
 * https://oeis.org/A185309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 31, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+(n+1)*a(n-2)+1 for n>1,
   a(0)=0, a(1)=0
   The function memoizes values so that repeated calls with arbitrary
   indices are fast. */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements currently stored */
static idx_t cache_cap = 0;   /* allocated capacity */

seq_t a(idx_t n)
{
    /* NULL handle: at least two elements for indices 0 and 1 */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Ensure the allocation succeeded */
        if (cache == NULL) {
            /* In a real program you'd handle this; here we just abort */
            abort();
        }
        cache[0] = 0;
        cache[1] = 0;
        cache_len = 2;
    }

    /* Resize array if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) {
            if (new_cap == 0)
                new_cap = 1;
            else
                new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 2) {
            cache[i] = 0;
        } else {
            /* Use seq_t everywhere to avoid overflows of intermediate casts */
            seq_t part1 = cache[i - 1];
            seq_t part2 = ((seq_t)(i + 1)) * cache[i - 2];
            cache[i] = part1 + part2 + 1;
        }
    }
    cache_len = n + 1;

    return cache[n];
}
