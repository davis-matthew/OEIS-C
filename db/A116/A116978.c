/*
 * A116978  Cubes whose multiplicative digital root is also a cube.
 * https://oeis.org/A116978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 1584, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A116978: Cubes whose multiplicative digital root is also a cube.
   a(n) is the n‑th non‑negative integer that is a perfect cube and has
   multiplicative digital root 0, 1 or 8.  The function builds a cache
   of cubes that satisfy the condition and returns the nth term. */

#include <stdlib.h>
#include "../utils/digit_product.h"

/* Cache of found cubes */
static seq_t *cube_cache      = NULL;
static size_t cube_cache_cap  = 0;
static size_t cube_cache_len  = 0;   /* number of valid entries in cache */

/* Next integer k whose cube will be considered */
static idx_t next_k = 0;

/* --------------------------------------------------------------------- */
/*  a(n)  :  return the nth term (1‑based).  n == 0 returns 0.
   The function is thread‑safe only in the sense that it uses static
   state; concurrent calls are undefined.  It works for all supported
   seq_t types.  --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* For n == 0 the definition is not required; return 0 */
    if (n == 0)
        return (seq_t)0;

    /* Populate the cache until we have at least n terms */
    while (cube_cache_len < (size_t)n) {
        seq_t k    = (seq_t)next_k;          /* convert to seq_t   */
        seq_t cube = k * k * k;              /* k³                 */

        /* Compute the multiplicative digital root of cube */
        seq_t r = cube;
        while (r >= (seq_t)10) {
            r = digit_product(r);            /* product of decimal digits */
        }

        /* Accept if the root is a perfect cube (0, 1, 8) */
        if (r == (seq_t)0 || r == (seq_t)1 || r == (seq_t)8) {
            /* Grow cache if necessary */
            if (cube_cache_len == cube_cache_cap) {
                size_t new_cap = cube_cache_cap ? cube_cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cube_cache, new_cap * sizeof(seq_t));
                if (!tmp)                                     /* allocation failure */
                    return (seq_t)0;
                cube_cache      = tmp;
                cube_cache_cap  = new_cap;
            }
            cube_cache[cube_cache_len++] = cube;         /* store cube            */
        }

        ++next_k;   /* move to next integer */
    }

    return cube_cache[n - 1];
}
