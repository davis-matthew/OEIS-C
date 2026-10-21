/*
 * A200195  Number of -n..n arrays x(0..5) of 6 elements with zero sum,
 *          adjacent elements differing by more than one, and elements
 *          alternately increasing and decreasing.
 * https://oeis.org/A200195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 68, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A200195: number of -n..n arrays x(0..5) of 6 elements with
   zero sum, adjacent elements differing by more than one,
   and elements alternately increasing and decreasing.  */
   
/* Empirical recurrence (valid for n >= 14):
   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + 2*a(n-5) - a(n-6)
          - a(n-7) + 2*a(n-8) - a(n-10) - 2*a(n-11) + 3*a(n-12) - a(n-13).
   Initial values a(1) .. a(13) are given below. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache     = NULL;   /* 1‑based cache, cache[0] unused */
static size_t cache_len = 0;      /* highest index currently stored */

/* initial values a(1)..a(13) */
static const seq_t init_vals[13] = {
    (seq_t)2,          /* a(1) */
    (seq_t)54,         /* a(2) */
    (seq_t)482,        /* a(3) */
    (seq_t)2240,       /* a(4) */
    (seq_t)7266,       /* a(5) */
    (seq_t)18838,      /* a(6) */
    (seq_t)41938,      /* a(7) */
    (seq_t)83600,      /* a(8) */
    (seq_t)153278,     /* a(9) */
    (seq_t)263198,     /* a(10) */
    (seq_t)428718,     /* a(11) */
    (seq_t)668684,     /* a(12) */
    (seq_t)1005790     /* a(13) */
};

/* Ensure cache has room for indices 1 .. idx (1‑based). */
static void expand_cache(size_t idx)
{
    if (idx <= cache_len) return;

    /* Allocate new cache array. */
    seq_t *new_cache = (seq_t*)malloc((idx + 1) * sizeof(seq_t));
    if (!new_cache) abort();                /* allocation failure */

    /* Copy any old values. */
    for (size_t i = 0; i <= cache_len; ++i)
        new_cache[i] = (cache ? cache[i] : (seq_t)0);

    /* Fill in missing terms. */
    for (size_t i = cache_len + 1; i <= idx; ++i) {
        if (i <= 13) {
            /* Initial values. */
            new_cache[i] = init_vals[i - 1];
            continue;
        }

        /* Compute a(i) using the empirical recurrence.  The
           positive part is summed first to avoid unsigned underflow. */
        seq_t pos = (seq_t)3 * new_cache[i - 1];
        pos += (seq_t)2 * new_cache[i - 5];
        pos += (seq_t)2 * new_cache[i - 8];
        pos += (seq_t)3 * new_cache[i - 12];

        seq_t neg = (seq_t)2 * new_cache[i - 2];
        neg += new_cache[i - 3];
        neg += new_cache[i - 6];
        neg += new_cache[i - 7];
        neg += new_cache[i - 10];
        neg += (seq_t)2 * new_cache[i - 11];
        neg += new_cache[i - 13];

        new_cache[i] = pos - neg;
    }

    /* Replace old cache. */
    free(cache);
    cache     = new_cache;
    cache_len = idx;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for 0, return 0 */

    size_t idx = (size_t)n;
    expand_cache(idx);
    return cache[idx];
}
