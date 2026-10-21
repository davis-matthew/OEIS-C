/*
 * A202440  Number of (n+2) X 3 binary arrays avoiding patterns 001 and 110 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A202440
   Formula: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4)  for n>5
   Initial values: a(1)=80, a(2)=138, a(3)=224, a(4)=338, a(5)=484 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base[5] = { (seq_t)80, (seq_t)138, (seq_t)224, (seq_t)338, (seq_t)484 };

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* return the pre‑computed initial values directly */
    if (n <= 5)
        return base[n - 1];

    /* initial allocation of the cache */
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t *)realloc(NULL, cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = base[i];
    }

    /* if the cache is too small, grow it */
    if (n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        /* compute the missing terms */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            seq_t val = 4 * cache[k - 2];
            val -= 6 * cache[k - 3];
            val += 4 * cache[k - 4];
            val -= cache[k - 5];
            cache[k - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
