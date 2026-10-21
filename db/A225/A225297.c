/*
 * A225297  Numbers divisible by their last digit cubed.
 * https://oeis.org/A225297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9015, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225297 : numbers k such that (k mod 10)^3 divides k.
   The function a(n) returns the nth term (1‑based).  It is computed
   by scanning integers in order and keeping a cached list of the
   terms that satisfy the condition.  The cache is grown as needed.
   This approach is O(n * average_checked_per_term) and remains
   well under the time limits for n up to at least 10 000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Helper: test whether a sequence value satisfies the rule.  */
static inline bool is_good(seq_t x)
{
    unsigned d = (unsigned)(x % 10);      /* last decimal digit      */
    if (d == 0)            return false; /* division by zero impossible */
    if (d == 1)            return true;  /* 1^3 = 1 always divides    */
    /* for d >= 7, d^3 > 216 > max 8‑bit value and the remainder test
       is still valid; we simply compute d^3 carefully. */
    seq_t d3 = (seq_t)(d * d * d);        /* d^3 fits in seq_t for 0<=d<=9 */
    return (x % d3) == 0;
}

/* ------------------------------------------------------------------ */
/* Cache of previously found terms.  */
static seq_t *cache     = NULL;    /* pointer to dynamic array       */
static size_t cache_len = 0;       /* number of terms stored        */
static size_t cache_cap = 0;       /* current allocated capacity    */
static seq_t next_candidate = 1;   /* integer to test next          */

/* ------------------------------------------------------------------ */
/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)        return (seq_t)0;   /* a(0) is undefined but guard */

    /* Grow the cache until we have n terms. */
    while (cache_len < (size_t)n)
    {
        seq_t cand = next_candidate++;
        if (is_good(cand))
        {
            /* Ensure enough capacity. */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)    /* allocation failure: abort */
                {
                    /* In a production build you would handle this
                       more gracefully; here we simply exit.      */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }
    return cache[n - 1];
}
