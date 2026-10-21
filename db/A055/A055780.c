/*
 * A055780  Number of symmetric types of (3,2n)-hypergraphs under action of
 *          complementing group C(3,2).
 * https://oeis.org/A055780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 58, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A055780: Number of symmetric types of (3,2n)-hypergraphs under action of complementing group C(3,2)
   Recurrence:
     a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - 2*a(n-5) + 2*a(n-7) - a(n-8)
   Initial values:
     a(0)=1, a(1)=7, a(2)=14, a(3)=35, a(4)=57, a(5)=98, a(6)=140, a(7)=210
*/
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure cache has entries [0..upto] stored */
static void ensure_cache(size_t upto)
{
    if (upto >= cache_cap) {
        /* Grow capacity (double until sufficient) */
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap <= upto) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Seed the first eight terms if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)35;
        cache[4] = (seq_t)57;
        cache[5] = (seq_t)98;
        cache[6] = (seq_t)140;
        cache[7] = (seq_t)210;
        cache_len = 8;
    }

    /* Compute remaining terms up to 'upto' */
    if (upto >= cache_len) {
        for (size_t i = cache_len; i <= upto; ++i) {
            cache[i] =  (seq_t)2 * cache[i-1]
                       - (seq_t)2 * cache[i-3]
                       + (seq_t)2 * cache[i-4]
                       - (seq_t)2 * cache[i-5]
                       + (seq_t)2 * cache[i-7]
                       - cache[i-8];
        }
        cache_len = upto + 1;
    }
}

seq_t a(idx_t n)
{
    /* Convert idx_t to size_t safely for array indexing */
    size_t idx = (size_t)n;
    ensure_cache(idx);
    return cache[idx];
}
