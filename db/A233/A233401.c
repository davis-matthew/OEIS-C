/*
 * A233401  Numbers k such that k^3 - b2 is a triangular number (A000217),
 *          where b2 is the largest square less than k^3.
 * https://oeis.org/A233401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A233401: numbers k such that k^3 − b2 is triangular,
   where b2 is the largest square strictly less than k^3.
   The function searches k sequentially, storing each valid k in a cache
   and returning the nth element.  Formula is implicit: a(n) is the nth
   integer k satisfying the condition. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;          /* cached valid k's */
static size_t cache_size = 0;        /* number of cached k's */
static size_t cache_cap  = 0;        /* allocated size of cache */
static idx_t next_k = 1;             /* k to test next */

/* helper: ensure cache capacity */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_buf;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* find enough terms */
    while (cache_size < n) {
        seq_t k   = (seq_t)next_k;
        seq_t cube   = k * k * k;
        seq_t m   = floor_sqrt(cube);          /* floor(sqrt(k^3)) */
        if (m * m == cube)                    /* k^3 is a perfect square */
            m--;                              /* use next smaller square */
        seq_t b2   = m * m;
        seq_t diff = cube - b2;                /* k^3 - largest square < k^3 */
        seq_t t    = diff * 8 + 1;             /* 8*diff+1 must be a square     */
        seq_t s    = floor_sqrt(t);
        if (s * s == t) {                     /* triangular check satisfied   */
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
