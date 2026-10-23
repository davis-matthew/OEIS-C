/*
 * A107307  Expansion of g.f. (1-x-2*x^2-x^3+x^4)/((x-1)^3*(6*x^2+2*x-1)).
 * https://oeis.org/A107307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 11*a(n-3) + 16*a(n-4) - 6*a(n-5)
 *            with initial values a(0)=1, a(1)=4, a(2)=15, a(3)=51, a(4)=183
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms      */
static idx_t  cache_size = 0;   /* number of terms stored (>=5) */

seq_t a(idx_t n)
{
    /* initialize cache with first five terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)51;
        cache[4] = (seq_t)183;
        cache_size = 5;
    }

    /* extend cache up to n if necessary */
    while (cache_size <= n) {
        /* allocate one more slot */
        seq_t *new_cache = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed – abort */
            abort();
        }
        cache = new_cache;

        /* compute next term using the recurrence */
        idx_t i = cache_size;               /* index of new term */
        seq_t next = 5 * cache[i - 1]
                     - 3 * cache[i - 2]
                     - 11 * cache[i - 3]
                     + 16 * cache[i - 4]
                     - 6 * cache[i - 5];
        cache[i] = next;
        cache_size++;
    }

    return cache[n];
}
