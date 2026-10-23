/*
 * A028006  Expansion of 1/((1-2x)(1-7x)(1-8x)(1-10x)).
 * https://oeis.org/A028006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 27*a(n-1) - 256*a(n-2) + 972*a(n-3) - 1120*a(n-4);
   a(0)=1, a(1)=27, a(2)=473, a(3)=6831. */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static const seq_t base_vals[4] = { 1, 27, 473, 6831 };

seq_t a(idx_t n)
{
    /* Return the pre‑defined values for n < 4 */
    if (n < 4) return base_vals[(size_t)n];

    /* Grow the cache if necessary */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – unlikely for small n */
        cache = tmp;

        /* Initialise the base elements if this is the first allocation */
        if (cache_len == 0) {
            for (size_t i = 0; i < 4; ++i) cache[i] = base_vals[i];
            cache_len = 4;
        }

        /* Compute new entries using the recurrence relation */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t v = cache[i-1] * 27;
            v += cache[i-3] * 972;
            v -= cache[i-2] * 256;
            v -= cache[i-4] * 1120;
            cache[i] = v;
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
