/*
 * A109403  Examine the sequence of all (even or odd) semiprimes, A001358, and
 *          record the averages of any pair of successive terms of the same
 *          parity.
 * https://oeis.org/A109403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 9258, u32 n <= 9284, u64 n <= 9284,
 *            u128 n <= 9284, big any n
 * Helpers:   is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A109403 – averages of successive semiprimes with the same parity
    (OEIS formula: a(n) = (s_k + s_{k+1}) / 2 for consecutive semiprimes s_k,
     s_{k+1} that have identical parity, listed in increasing order) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_semiprime_uint64.h"

/* cache of previously computed sequence values, kept between calls   */
static seq_t *cache      = NULL;
static idx_t cache_cap   = 0;    /* current allocation size           */
static idx_t cache_size  = 0;    /* number of values already stored   */

/* state of the semiprime generator, kept between calls            */
static bool        have_last   = false;
static uint64_t    last_sp     = 0;        /* last encountered semiprime   */
static int         last_parity = 0;        /* parity of last_sp (0 even, 1 odd) */
static uint64_t    candidate   = 4;        /* next number to test as a semiprime */

/* helper: grow the cache array when needed                         */
static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_mem == NULL) {
        /* In an OEIS evaluation environment we abort on OOM. */
        abort();
    }
    cache = new_mem;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; produce 0 for n == 0 (unused)              */
    if (n == 0)
        return (seq_t)0;

    /* Return a cached value if already available                   */
    if (cache_size >= n)
        return cache[n - 1];

    /* Ensure some initial cache capacity                          */
    if (cache_cap == 0)
        grow_cache(16);

    /* Generate terms until we have at least n of them              */
    while (cache_size < n) {
        /* Find the next semiprime                                 */
        while (!is_semiprime_uint64(candidate))
            ++candidate;

        int parity = (int)(candidate & 1);

        /* If the current and previous semiprimes have the same
           parity, compute their average and store it in the cache.*/
        if (have_last && parity == last_parity) {
            seq_t avg = ((seq_t)last_sp + (seq_t)candidate) / (seq_t)2;
            if (cache_size == cache_cap)
                grow_cache(cache_cap * 2);
            cache[cache_size++] = avg;
        }

        /* Advance the generator state                             */
        last_sp      = candidate;
        last_parity  = parity;
        have_last    = true;
        ++candidate;
    }

    return cache[n - 1];
}
