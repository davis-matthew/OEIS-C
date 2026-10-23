/*
 * A140420  Binomial transform of 0, 1, 1, 7, 7, 31, 31, ..., zero followed by
 *          duplicated A083420.
 * https://oeis.org/A140420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A140420
   Formula: a(n) = (3^(n+1) - (-1)^n + 2)/4 - 2^n
   Recurrence: a(0)=0, a(1)=1,
   for k>=0: a(k+1) = 3*a(k) + 2^k + (-1)^k - 1
   (which follows from the explicit formula).
   This implementation builds the sequence on demand and caches
   previous values.  The cache grows geometrically, so each value
   is computed only once.  All arithmetic is performed with
   unsigned integers of type seq_t so it works correctly for
   both fixed‑size and arbitrary‑precision builds. */

#include <stddef.h>           /* size_t */

static seq_t *a_cache   = NULL;   /* cache of a(n) */
static seq_t *pow2_cache = NULL;   /* cache of 2^n */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t  cache_len = 0;      /* next index to compute */

/* Ensure that a_cache[i] and pow2_cache[i] exist for all i <= n. */
static void
ensure_cache(idx_t n)
{
    if (cache_len == 0) { /* first time */
        cache_cap = 4;
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        pow2_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        a_cache[0]    = 0;
        pow2_cache[0] = 1;
        cache_len = 1;                 /* next index to compute */
    }

    /* Grow the allocation if necessary */
    while (cache_cap <= (size_t)n + 1) {
        cache_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, cache_cap * sizeof(seq_t));
        pow2_cache = (seq_t *)realloc(pow2_cache, cache_cap * sizeof(seq_t));
    }

    /* Fill missing entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i > 0)
            pow2_cache[i] = pow2_cache[i - 1] * 2;   /* 2^i */

        if (i == 0) {          /* a(0) already set */
            a_cache[0] = 0;
            continue;
        }
        if (i == 1) {          /* a(1) must be 1 */
            a_cache[1] = 1;
            continue;
        }

        /* Recurrence uses k = i-1 */
        idx_t k = i - 1;
        seq_t prev = a_cache[k];
        seq_t rhs  = pow2_cache[k];        /* 2^k */

        /* add (-1)^k - 1 */
        if ((k & 1) == 0) /* even k: (+1) */
            rhs += 1;
        else               /* odd k: (-1) */
            rhs += (seq_t)(-1);
        rhs -= 1;                         /* subtract 1 */

        a_cache[i] = prev * 3 + rhs;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
