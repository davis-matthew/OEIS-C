/*
 * A066733  Numbers such that the product of the digits of its square is a
 *          square > 0.
 * https://oeis.org/A066733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 927 (2026-11-03)
 * Helpers:   digit_product, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers x for which digit_product(x*x) is a perfect square > 0
   (computed by a simple incremental search).  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/digit_product.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* --------------------------------------------------------------------- */
/* Cache handling                                               */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;          /* stored terms for n >= 1           */
static size_t cache_size = 0;        /* current number of cached terms    */
static size_t cache_cap   = 0;        /* allocated array capacity          */
static seq_t next_candidate = 1;     /* next integer to test for property*/

static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? (cache_cap * 2) : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(1);   /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Property test                                                 */
/* --------------------------------------------------------------------- */
static bool passes(seq_t x)
{
    seq_t sq   = x * x;
    seq_t prod = digit_product(sq);
    if (prod == 0) return false;
    seq_t r = floor_sqrt(prod);
    return r * r == prod;
}

/* --------------------------------------------------------------------- */
/* Public function: returns a(n), 1‑based index                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;          /* out of range: caller should not use n<1 */

    /* grow cache until it contains at least n terms            */
    while (cache_size < (size_t)n) {
        if (passes(next_candidate)) {
            ensure_cache(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[(size_t)n - 1];
}
