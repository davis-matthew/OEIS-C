/*
 * A166397  Multiples of 13 whose reversal - 1 is also a multiple of 13.
 * https://oeis.org/A166397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 397, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the n-th positive integer that is a multiple of 13 and whose
   decimal reversal minus 1 is also a multiple of 13. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;          /* last k processed (so that 13*k was tested) */

/* Reverse the decimal digits of a value of type seq_t. */
static seq_t rev_seq(seq_t v)
{
    seq_t r = (seq_t)0;
    while (v > (seq_t)0) {
        r = r * (seq_t)10 + (v % (seq_t)10);
        v = v / (seq_t)10;
    }
    return r;
}

/* Grow the cache to hold at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* undefined for offset 1, but safe to return 0 */
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        ++last_k;                                      /* next multiple of 13 */
        seq_t m   = (seq_t)13 * (seq_t)last_k;         /* m = 13 * last_k */
        seq_t r = rev_seq(m);
        if ((r % (seq_t)13) == (seq_t)1) {             /* check the divisibility condition */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                grow_cache(new_cap);
            }
            cache[cache_len++] = m;
        }
    }

    return cache[(size_t)(n - 1)];   /* zero‑based indexing in the cache */
}
