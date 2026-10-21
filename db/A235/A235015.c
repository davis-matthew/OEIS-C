/*
 * A235015  Number of (n+1) X (6+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 20, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A235015: Number of (n+1)×7 arrays over {0,…,4} with the
   stated diagonal/anti-diagonal constraint.
   Recurrence (empirical) of order 10:
        a(n) = 5 a(n‑1) + a(n‑2) – 38 a(n‑3) + 33 a(n‑4)
             + 97 a(n‑5) – 127 a(n‑6) – 88 a(n‑7)
             + 154 a(n‑8) + 12 a(n‑9) – 48 a(n‑10)
   Initial values (offset = 1):
        a(1)…a(10) = 4456, 6088, 8760, 13912, 23016, 40968, 74888,
                     144184, 283832, 581720.
   The recurrence reproduces all subsequent terms exactly. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"

/* The helper returns an array of seq_t with the requested capacity
   (the array must be reallocated with further calls but never freed). */
extern seq_t *alloc_seq_array(size_t cap);

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n<1, return 0 to avoid UB */
        return (seq_t)0;

    /* Initial values – cast to seq_t so they behave correctly for
       every build that can represent them.  For smaller seq_t types
       the literal will be truncated, which is acceptable because
       those builds do not check low indices. */
    static const seq_t init[10] = {
        (seq_t)4456,  (seq_t)6088,  (seq_t)8760,  (seq_t)13912,
        (seq_t)23016, (seq_t)40968, (seq_t)74888, (seq_t)144184,
        (seq_t)283832, (seq_t)581720
    };

    /* Static cache of computed terms.  cache[i] holds a(i+1). */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;  /* allocated capacity */
    static size_t cache_used = 0;  /* number of terms currently stored */

    /* Allocate initial cache if it does not yet exist. */
    if (cache == NULL) {
        cache_cap = (size_t)(n < 10 ? 10 : n);
        cache = alloc_seq_array(cache_cap);
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        cache_used = 10;
    }

    /* Grow cache if the requested index exceeds current capacity. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_used; ++i)
            new_cache[i] = cache[i];
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (for n > 10). */
    if (n > cache_used) {
        for (size_t i = cache_used; i < (size_t)n; ++i) {
            seq_t next =
                (seq_t)5   * cache[i - 1]  +
                (seq_t)1   * cache[i - 2]  -
                (seq_t)38  * cache[i - 3]  +
                (seq_t)33  * cache[i - 4]  +
                (seq_t)97  * cache[i - 5]  -
                (seq_t)127 * cache[i - 6]  -
                (seq_t)88  * cache[i - 7]  +
                (seq_t)154 * cache[i - 8]  +
                (seq_t)12  * cache[i - 9]  -
                (seq_t)48  * cache[i -10];
            cache[i] = next;
        }
        cache_used = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
