/*
 * A117686  Squares for which the product of the digits are cubes.
 * https://oeis.org/A117686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th perfect square such that the product of its
   decimal digits is a perfect cube.  The function generates the sequence
   by iterating over square numbers and selecting those that satisfy the
   product‑cube condition. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* safety, though n ≥ 1 per OEIS */

    static seq_t *cache = NULL;         /* cached sequence values       */
    static size_t cap   = 0;            /* current capacity of cache    */
    static size_t len   = 0;            /* number of values stored      */
    static idx_t  next_i = 0;           /* next integer whose square we
                                           will test                  */

    while (len < n) {                   /* generate until we have n terms */
        seq_t sq = (seq_t)next_i * (seq_t)next_i;
        seq_t prod = digit_product(sq);
        seq_t r = floor_cube_root(prod);
        if (r * r * r == prod) {        /* product is a perfect cube?   */
            if (len == cap) {           /* grow the cache if needed     */
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return 0;     /* allocation failure          */
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = sq;          /* store qualifying square      */
        }
        ++next_i;                      /* move to next integer         */
    }
    return cache[n - 1];                /* return the requested term    */
}
