/*
 * A124759  Sum of products of consecutive terms for compositions in standard
 *          order.
 * https://oeis.org/A124759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9437, u16 n <= 9437, u32 n <= 9437, u64 n <= 9437,
 *            u128 n <= 9437, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A124759 – Sum of products of consecutive terms for compositions 
   in standard order. 
   Sequence is indexed from 0; a(0) = 0 (empty composition). 
   For a composition `b(1)...b(k)` the value is
          a = Σ_{i=1}^{k-1} b(i) * b(i+1).
   Compositions are enumerated in “standard order”: 
   all compositions of the integer `s` are listed for s = 0,1,2,…, 
   and for each fixed `s` they are listed in descending lexicographic
   order of their parts. This matches OEIS A066099.
*/

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cached values a(0), a(1), … */
static size_t cache_cap = 0;         /* allocated capacity of cache     */
static idx_t  cache_count = 0;       /* number of terms currently cached */
static idx_t  next_sum   = 0;        /* next integer `s` to generate */
static int    comp[64];              /* temporary storage for a composition */

/* Make sure the cache has room for at least `need` indices */
static void ensure_cache_space(idx_t need)
{
    if (need + 1 > (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)(need + 1))
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – terminate */
            abort();
        }
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Recursive generator for compositions of `rem` with length so far `len` */
static void gen_rec(int rem, int len)
{
    if (rem == 0) {
        /* Compute a(n) for the current composition */
        seq_t val = 0;
        for (int i = 0; i + 1 < len; ++i)
            val += (seq_t)comp[i] * (seq_t)comp[i + 1];
        ensure_cache_space(cache_count);
        cache[cache_count++] = val;
        return;
    }
    for (int part = rem; part >= 1; --part) {
        comp[len] = part;
        gen_rec(rem - part, len + 1);
    }
}

/*
 * Public function: a(idx_t n)
 * Returns the nth term of OEIS A124759.
 */
seq_t a(idx_t n)
{
    while (cache_count <= n) {
        /* Generate all compositions of current sum `next_sum` */
        gen_rec((int)next_sum, 0);
        ++next_sum;
    }
    return cache[n];
}
