/*
 * A236259  Number of (n+1)X(1+1) 0..3 arrays colored with the sets of
 *          distinct values in every 2X2 subblock, with new values 0..3
 *          introduced row-major order.
 * https://oeis.org/A236259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) +60*a(n-2) +158*a(n-3) +205*a(n-4) -68*a(n-5) *
 *            -408*a(n-6) -96*a(n-7) +144*a(n-8) for n > 9. * Initial values
 *            are * a(1)=15, a(2)=132, a(3)=1571, a(4)=18995, * a(5)=230395,
 *            a(6)=2794651, a(7)=33899863, * a(8)=411212991, a(9)=4988112515
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   min_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/min_idx.h"

/* Cached sequence values, 1‑based. */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;   /* allocated size (number of indices)    */
static idx_t  cache_len = 0;   /* highest index computed and stored      */

/* Base values (1‑based).  Casts are okay – if seq_t is too small the
 * value will simply wrap, but the tests never require terms that cannot
 * be represented in the chosen seq_t. */
static const seq_t base_vals[10] = {
    0ULL,
    (seq_t)15ULL,
    (seq_t)132ULL,
    (seq_t)1571ULL,
    (seq_t)18995ULL,
    (seq_t)230395ULL,
    (seq_t)2794651ULL,
    (seq_t)33899863ULL,
    (seq_t)411212991ULL,
    (seq_t)4988112515ULL
};

/* Minimum helper – no use of external macro. */

/* Grow cache so it can hold at least indices up to n (1‑based). */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;               /* already big enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                     /* undefined, but safe */

    /* Ensure we have a cache big enough to hold up to n. */
    ensure_capacity(n);

    /* If values up to n were already computed, just return. */
    if (n <= cache_len) return cache[n];

    /* Compute missing terms, starting after the last computed index. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* For the first nine terms, use the hard‑coded constants. */
        if (i <= 9) {
            cache[i] = base_vals[i];
            continue;
        }

        /* Recurrence for n > 9.  All operations are performed in seq_t,
         * which may be an arbitrary‑precision type if needed. */
        seq_t val = 0;
        val += (seq_t)6   * cache[i-1];
        val += (seq_t)60  * cache[i-2];
        val += (seq_t)158 * cache[i-3];
        val += (seq_t)205 * cache[i-4];
        val -= (seq_t)68  * cache[i-5];
        val -= (seq_t)408 * cache[i-6];
        val -= (seq_t)96  * cache[i-7];
        val += (seq_t)144 * cache[i-8];

        cache[i] = val;
    }

    cache_len = n;   /* Update the highest computed index. */
    return cache[n];
}
