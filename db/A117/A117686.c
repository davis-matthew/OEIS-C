/*
 * A117686  Squares for which the product of the digits are cubes.
 * https://oeis.org/A117686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 8951 (2026-11-03)
 * Helpers:   digit_product, ipow_i32, mem
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
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
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
