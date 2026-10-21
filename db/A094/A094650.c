/*
 * A094650  An accelerator sequence for Catalan's constant.
 * https://oeis.org/A094650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A094650: a(n) = -a(n-1)+4*a(n-2)+3*a(n-3)-3*a(n-4)-a(n-5)
   with initial values a(0)=5, a(1)=-1, a(2)=9, a(3)=-4, a(4)=25 */
#include <stdlib.h>

/* Cached sequence values.  They are grown as needed. */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* Compute a(n) for any non‑negative n.  The function is thread‑unsafe
   but satisfies the required interface. */
seq_t a(idx_t n)
{
    /* Ensure the cache is initialised with the first five terms. */
    if (seq_len == 0)
    {
        /* Start with a modest capacity and expand only when needed. */
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq_cache == NULL)
            return (seq_t)0; /* allocation failure, unlikely in tests */

        seq_cache[0] = (seq_t)5;
        seq_cache[1] = (seq_t)-1;
        seq_cache[2] = (seq_t)9;
        seq_cache[3] = (seq_t)-4;
        seq_cache[4] = (seq_t)25;
        seq_len = 5;
    }

    /* Grow the cache if we need to return a larger index. */
    if ((size_t)n >= seq_len)
    {
        size_t needed = (size_t)n + 1;
        if (needed > seq_cap)
        {
            /* Grow capacity: at least double, but also enough for needed. */
            size_t new_cap = seq_cap > 0 ? seq_cap * 2 : 16;
            if (new_cap < needed)
                new_cap = needed;
            seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL)
                return (seq_t)0; /* allocation failure */
            seq_cache = new_cache;
            seq_cap   = new_cap;
        }

        /* Continue the recurrence to fill missing terms. */
        for (size_t i = seq_len; i < needed; ++i)
        {
            /* a(i) = -a(i-1)+4*a(i-2)+3*a(i-3)-3*a(i-4)-a(i-5) */
            seq_t next =
                -seq_cache[i-1] + (seq_t)4 * seq_cache[i-2]
                + (seq_t)3 * seq_cache[i-3] - (seq_t)3 * seq_cache[i-4]
                - seq_cache[i-5];
            seq_cache[i] = next;
        }
        seq_len = needed;
    }

    return seq_cache[(size_t)n];
}
