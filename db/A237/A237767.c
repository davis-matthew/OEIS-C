/*
 * A237767  Integers whose product of digits is a nonzero cube.
 * https://oeis.org/A237767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements brute‑force generation of OEIS A237767: integers
    whose product of digits is a nonzero perfect cube.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/floor_cube_root.h"

static seq_t *seq_realloc(seq_t *old, size_t /*old_cap*/, size_t new_cap)
{
    /* realloc returns void*, cast to seq_t* for C/C++ compatibility */
    return (seq_t *)realloc(old, new_cap * sizeof(seq_t));
}

/* Return the nth element (1‑based) of OEIS A237767. */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;   /* cached terms            */
    static size_t cache_cap  = 0;      /* allocated capacity      */
    static idx_t  cache_len  = 0;      /* number of terms cached  */
    static idx_t  last_checked = 0;    /* last integer examined   */

    if (n == 0)
        return 0;                     /* undefined, out of range */

    /* generate terms until we have the nth one */
    while (cache_len < n) {
        /* examine next integer */
        last_checked++;

        /* product of digits */
        seq_t prod = digit_product((seq_t)last_checked);
        if (prod == (seq_t)0)          /* contains zero – skip */
            continue;

        /* test whether prod is a perfect cube */
        seq_t root = floor_cube_root(prod);
        if (root * root * root == prod) {   /* cube found */
            /* grow cache if needed */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL)
                    abort();                         /* out of memory */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
