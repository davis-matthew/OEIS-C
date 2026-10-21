/*
 * A219447  Unchanging value maps: number of nX4 binary arrays indicating the
 *          locations of corresponding elements unequal to no king-move
 *          neighbor in a random 0..2 nX4 array.
 * https://oeis.org/A219447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for A219447 a(n) = 5*a(n-1) -7*a(n-2)
 *            +8*a(n-3) +20*a(n-4) -22*a(n-5) +6*a(n-6) +34*a(n-7) +15*a(n-8)
 *            -21*a(n-9) -28*a(n-10) -35*a(n-11) -9*a(n-12) -a(n-13) for n>14
 *            Initial values: a(1)=7, a(2)=7, a(3)=43, a(4)=169, a(5)=665,
 *            a(6)=2603, a(7)=10421, a(8)=41777, a(9)=166911, a...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL;   /* array of computed values          */
static size_t cache_cap  = 0;      /* allocated capacity (number of slots) */
static size_t cache_len  = 0;      /* number of values currently stored   */
static int    cache_init = 0;      /* 0=not initialized, 1=initialized   */

/* Grow the cache to hold at least 'need' slots.  Previous contents are
   preserved.  Works for any SeqT that can be copied with assignment. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;

    seq_t *new_buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_buf) abort();          /* allocation failure */

    /* Copy existing values to new buffer.  Use assignment to handle
       non‑trivial types correctly. */
    for (size_t i = 0; i < cache_len; ++i)
        new_buf[i] = cache[i];

    /* Free old buffer. */
    free(cache);
    cache        = new_buf;
    cache_cap    = new_cap;
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥1 by the caller.  Ensure the cache holds
       the initial values exactly once. */
    if (!cache_init) {
        const size_t init_cnt = 14;
        static const seq_t init_vals[14] = {
            7,  7,   43,    169,   665,   2603,
            10421, 41777, 166911, 665377,
            2653983, 10593377, 42287115, 168773051
        };

        ensure_cache_cap(init_cnt);
        for (size_t i = 0; i < init_cnt; ++i)
            cache[i] = init_vals[i];
        cache_len  = init_cnt;
        cache_init = 1;
    }

    /* Convert index to size_t (safe for n up to 10000). */
    size_t idx = (size_t)n;
    if (idx == 0)              /* caller should never use n=0, but guard. */
        return (seq_t)0;

    /* Return cached value if we already have it. */
    if (idx <= cache_len)
        return cache[idx - 1];

    /* Compute new terms up to desired index. */
    ensure_cache_cap(idx);

    for (size_t i = cache_len; i < idx; ++i) {
        /* i is the 0‑based index of the next value to compute:
           a(i+1).  All references using cache[ ] are valid because
           i >= 13 (the cache has at least 14 elements). */
        seq_t val =
              (seq_t)5  * cache[i - 1]            /* a(i)   */
            - (seq_t)7  * cache[i - 2]            /* a(i-1) */
            + (seq_t)8  * cache[i - 3]            /* a(i-2) */
            + (seq_t)20 * cache[i - 4]            /* a(i-3) */
            - (seq_t)22 * cache[i - 5]            /* a(i-4) */
            + (seq_t)6  * cache[i - 6]            /* a(i-5) */
            + (seq_t)34 * cache[i - 7]            /* a(i-6) */
            + (seq_t)15 * cache[i - 8]            /* a(i-7) */
            - (seq_t)21 * cache[i - 9]            /* a(i-8) */
            - (seq_t)28 * cache[i - 10]           /* a(i-9) */
            - (seq_t)35 * cache[i - 11]           /* a(i-10) */
            - (seq_t)9  * cache[i - 12]           /* a(i-11) */
            -           cache[i - 13];            /* a(i-12) */

        cache[i] = val;
    }

    cache_len = idx;
    return cache[idx - 1];
}
