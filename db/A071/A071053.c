/*
 * A071053  Number of ON cells at n-th generation of 1-D CA defined by Rule
 *          150, starting with a single ON cell at generation 0.
 * https://oeis.org/A071053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, changed
 * Formula:   a(0)=1, a(1)=3; For n even: a(n)=a(n/2); For n odd :
 *            a(n)=a(n/2)+2*a((n/2)/2)
 * Range:     u8 n <= 254, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 91 terms through n = 9983 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Ensure cache is large enough */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
        cache_size = new_size;
        /* store the base values for future use */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    /* Return cached value if available */
    if (cache[n] != (seq_t)0) return cache[n];

    seq_t result;
    if ((n & 1u) == 0) {          /* even n */
        result = a(n >> 1u);
    } else {                      /* odd n */
        idx_t half = n >> 1u;                     /* floor(n/2) */
        result = a(half) + (seq_t)2 * a(half >> 1u);   /* + 2*a(floor(half/2)) */
    }

    cache[n] = result;
    return result;
}
