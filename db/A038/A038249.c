/*
 * A038249  Third line of 4-wave sequence A038197.
 * https://oeis.org/A038249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038249 - Third line of 4-wave sequence A038197
   Recurrence:
      a(n) = 10*a(n-2) - 15*a(n-4) + 7*a(n-6) - a(n-8)   (n ≥ 8)
   Initial values:
      a(0)=1, a(1)=2, a(2)=9, a(3)=19, a(4)=75, a(5)=160,
      a(6)=622, a(7)=1329
*/

#include <stddef.h>
#include "../utils/alloc_seq.h"   /* declares seq_t *alloc_seq(size_t);  */
#include "../utils/free_seq.h"    /* declares void free_seq(seq_t *);   */

static seq_t *cache = 0;        /* memoised values for n ≥ 8          */
static idx_t  cache_size = 0;   /* number of cached elements          */

/* Compute a(n) for all indices up to n and store them in cache   */
static void compute_to(idx_t n)
{
    if (cache_size > n)             /* already computed up to n */
        return;

    /* Remember old state before reallocating */
    idx_t      old_size   = cache_size;
    seq_t *    old_cache  = cache;
    seq_t *    new_cache  = alloc_seq((size_t)n + 1);
    if (!new_cache)                /* allocation failure      */
        return;

    /* Copy existing values */
    for (idx_t i = 0; i < old_size; ++i)
        new_cache[i] = old_cache[i];
    free_seq(old_cache);

    cache = new_cache;
    cache_size = (idx_t)(n + 1);

    /* Initialise the first eight terms if this is the first run   */
    if (old_size == 0)
    {
        const seq_t init[8] =
        { (seq_t)1, (seq_t)2, (seq_t)9, (seq_t)19,
          (seq_t)75, (seq_t)160, (seq_t)622, (seq_t)1329 };
        for (idx_t i = 0; i < 8 && i < cache_size; ++i)
            cache[i] = init[i];
    }

    /* Compute remaining terms using the recurrence            */
    idx_t start = old_size > 8 ? old_size : 8;
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t v = cache[i - 2] * (seq_t)10;
        v = v - cache[i - 4] * (seq_t)15;
        v = v + cache[i - 6] * (seq_t)7;
        v = v - cache[i - 8];
        cache[i] = v;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* Handle the first eight terms directly */
    if (n < 8)
    {
        static const seq_t init[8] =
        { (seq_t)1, (seq_t)2, (seq_t)9, (seq_t)19,
          (seq_t)75, (seq_t)160, (seq_t)622, (seq_t)1329 };
        return init[(size_t)n];
    }

    compute_to(n);
    return cache[n];
}
