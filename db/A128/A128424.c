/*
 * A128424  a(n) = floor(sqrt(a(n-1)^2 + a(n-2)^2 + a(n-1)*a(n-2))), a(1)=1,
 *          a(2)=3.
 * https://oeis.org/A128424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A128424 – a(n) = floor(sqrt(a(n-1)^2 + a(n-2)^2 + a(n-1)*a(n-2)))
    a(1)=1, a(2)=3
    For n>7 the sequence satisfies the linear recurrence
    a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4)
    (valid for all n>7).
*/
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*  Cache for memoised values.  Index 0 corresponds to a(1). */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;   /* number of entries currently allocated */
static size_t cache_len = 0;   /* number of entries that have a valid value */

/*  Ensure the cache can hold at least `needed` entries (indexed 0 … needed-1).  */
static void
ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* out of memory – abort the program */
        exit(1);
    }
    /* initialise the newly allocated part to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;
    cache     = tmp;
    cache_cap = new_cap;
}

/*  First seven terms (indices 0 … 6 correspond to a(1)…a(7)).  */
static const seq_t first_seven[7] = {
    (seq_t)1,   /* a(1) */
    (seq_t)3,   /* a(2) */
    (seq_t)3,   /* a(3) */
    (seq_t)5,   /* a(4) */
    (seq_t)7,   /* a(5) */
    (seq_t)10,  /* a(6) */
    (seq_t)14   /* a(7) */
};

/*  Sequence function.  */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not used, but defensive */

    /* Convert to unsigned size_t for array indexing.  */
    size_t target = (size_t)n;

    /* Grow the cache if necessary.  */
    ensure_cache(target);

    /* Initialise any missing first‑seven terms.  */
    for (size_t i = 0; i < 7 && i < target; ++i) {
        if (cache_len <= i && cache[i] == (seq_t)0)
            cache[i] = first_seven[i];
        if (cache_len < i + 1)
            cache_len = i + 1;
    }

    /* Compute further terms, using the linear recurrence for i >= 7.  */
    for (size_t i = cache_len; i < target; ++i) {
        /* For indices 0…6 (i < 7) the terms are already set above.  */
        if (i < 7) {
            cache[i] = first_seven[i];
            continue;
        }

        /*  a(i+1) = 2*a(i) - a(i-1) + a(i-2) - a(i-3)  (zero‑based indices)  */
        seq_t t1 = cache[i - 1];                 /* a(n-1) */
        seq_t t2 = cache[i - 2];                 /* a(n-2) */
        seq_t t3 = cache[i - 3];                 /* a(n-3) */
        seq_t t4 = cache[i - 4];                 /* a(n-4) */

        seq_t val = 2 * t1;              /* 2*a(n-1) */
        val -= t2;                       /* - a(n-2) */
        val += t3;                       /* + a(n-3) */
        val -= t4;                       /* - a(n-4) */
        cache[i] = val;
    }

    return cache[target - 1];
}
