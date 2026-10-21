/*
 * A226940  a(0)=0; if a(n-1) is odd, a(n) = n + a(n-1), otherwise a(n) = n -
 *          a(n-1).
 * https://oeis.org/A226940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; if a(n-1) is odd, a(n)=n + a(n-1),
   otherwise a(n)=n – a(n-1).  Index n is 0‑based. */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Memoisation buffer.  All functions below use only these globals.  */
/* ------------------------------------------------------------------ */
static seq_t *cache   = NULL;   /* cache[0 .. cache_len-1] holds a(0..cache_len-1) */
static size_t cache_len = 0;    /* number of stored terms (0 means not yet allocated) */
static size_t cache_cap = 0;    /* allocated capacity of cache */

/* Ensure that the cache can hold at least 'need' elements.  If
   reallocation fails, the program aborts (as is standard for many
   competitive‑programming solutions). */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)     /* already sufficient */
        return;

    /* increase capacity if needed; the first allocated size is 16,
       afterwards it doubles each time. */
    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap < need)
        cache_cap <<= 1;   /* grow by factor of two */

    seq_t *new_mem = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!new_mem)          /* realloc failed */
        abort();
    cache = new_mem;
}

/* ------------------------------------------------------------------ */
/* The required function.  It is safe to call in any order and will
   compute terms lazily, reusing already computed values.           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* required number of terms */
    size_t required = (size_t)n + 1;
    if (required == 0)        /* n was MAX_UINT64 and wrapped to 0 */
        required = 1;

    /* Allocate memory if it hasn't been done yet, or expand it if needed. */
    ensure_capacity(required);

    /* Compute terms up to 'required-1' if they haven't been computed yet. */
    if (cache_len < required) {
        /* If this is the very first call, initialise the first term. */
        if (cache_len == 0) {
            cache[0] = 0;            /* a(0) = 0 */
            cache_len = 1;
        }

        /* Generate remaining terms in ascending order. */
        for (idx_t i = (idx_t)cache_len; i < (idx_t)required; ++i) {
            seq_t prev = cache[i - 1];
            seq_t cur;
            /* Test oddness without sign problems: bitwise & with 1 works for
               negative numbers as well. */
            if ((prev & 1) != 0)     /* prev is odd */
                cur = (seq_t)i + prev;
            else                     /* prev is even */
                cur = (seq_t)i - prev;
            cache[i] = cur;
        }
        cache_len = required;
    }

    return cache[n];
}
