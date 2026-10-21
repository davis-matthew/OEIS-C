/*
 * A205817  Number of (n+1) X 3 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock unequal to the number of
 *          counterclockwise edge increases.
 * https://oeis.org/A205817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) – a(n-2) – 4*a(n-3) + 2*a(n-4) (n ≥ 5) initial
 *            values: a(1)=66, a(2)=216, a(3)=714, a(4)=2430
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;    /* cache[1] .. cache[n] */
static size_t cache_size = 0;      /* allocated size (number of slots) */
static size_t computed = 0;        /* largest index computed so far */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1 */

    /* initialise cache on the first call */
    if (!seq_cache) {
        cache_size = 5;                      /* slots 0..4 (index 0 unused) */
        seq_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!seq_cache) return 0;
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)66;
        seq_cache[2] = (seq_t)216;
        seq_cache[3] = (seq_t)714;
        seq_cache[4] = (seq_t)2430;
        computed = 4;
    }

    /* grow the cache if required */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size;
        while ((size_t)n >= new_size) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;
        seq_cache = tmp;
        cache_size = new_size;
    }

    /* return a previously computed value */
    if ((size_t)n <= computed) return seq_cache[(size_t)n];

    /* compute missing terms up to n using the recurrence */
    for (size_t k = computed + 1; k <= (size_t)n; ++k) {
        seq_t val = 4 * seq_cache[k - 1]
                    - seq_cache[k - 2]
                    - 4 * seq_cache[k - 3]
                    + 2 * seq_cache[k - 4];
        seq_cache[k] = val;
    }
    computed = (size_t)n;

    return seq_cache[(size_t)n];
}
