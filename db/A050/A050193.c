/*
 * A050193  a(n)=a(n-1)+a(n-2)-d, where d=a(n/3) if 3 divides n, else d=0; 2
 *          initial terms.
 * https://oeis.org/A050193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-(3|n?a(n/3):0), a(1) = a(2) = 1
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, realloc */

static seq_t *cache      = NULL;
static idx_t  capacity   = 0;
static idx_t  computed   = 0;   /* highest n computed */

seq_t a(idx_t n)
{
    /* Undefined for n == 0 – return 0 safely */
    if (n == 0)
        return (seq_t)0;

    /* Base cases – these indices are guaranteed to exist */
    if (n <= 2)
        return (seq_t)1;

    /* Initial allocation */
    if (cache == NULL) {
        capacity = 3;              /* we need indices 0,1,2 at minimum */
        cache    = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[1] = cache[2] = (seq_t)1;
        computed = 2;
    }

    /* Grow the cache if necessary */
    if (n > computed) {
        if (n >= capacity) {      /* need more space */
            idx_t new_cap = n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);
            cache    = tmp;
            capacity = new_cap;
        }

        /* Compute the missing terms iteratively */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t val = cache[i - 1] + cache[i - 2];
            if ((i % 3) == 0)       /* subtract a(i/3) when 3 divides i */
                val -= cache[i / 3];
            cache[i] = val;
        }
        computed = n;
    }

    return cache[n];
}
