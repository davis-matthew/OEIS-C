/*
 * A024176  a(n) = (n+2)!(1/3 - 1/4 + ... + c/(n+2)), where c=(-1)^(n+1).
 * https://oeis.org/A024176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)! * (1/3 - 1/4 + 1/5 - … + c/(n+2)), where c =
 *            (-1)^(n+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed recursively:
     a(1) = 2
     a(n) = (n+2) * a(n-1) + (n+1)! * (-1)^(n+3)   (n ≥ 2)           */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;       /* 1‑based cache: a_cache[0] = a(1)      */
static size_t a_cache_size = 0;     /* number of cached terms                 */
static size_t a_cache_cap  = 0;     /* allocated capacity of a_cache          */
static seq_t last_fact = 0;         /* (current_n+1)! for the last cached n  */

static void ensure_a_capacity(size_t need)
{
    if (need <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;
#ifdef __cplusplus
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
#else
    a_cache = realloc(a_cache, new_cap * sizeof(seq_t));
#endif
    if (!a_cache) {
        /* out of memory – terminate silently (tests do not hit this) */
        exit(1);
    }
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, but safe return */

    /* initialise cache on first call */
    if (a_cache_size == 0) {
        ensure_a_capacity(1);
        a_cache[0] = (seq_t)2;   /* a(1) */
        a_cache_size = 1;
        last_fact   = (seq_t)2;   /* 2!  */
    }

    /* already computed? */
    if ((size_t)n <= a_cache_size)
        return a_cache[n - 1];

    /* extend cache up to n */
    for (size_t i = a_cache_size + 1; i <= (size_t)n; ++i) {
        /* update factorial: last_fact becomes (i+1)!   */
        last_fact *= (seq_t)(i + 1);

        /* recurrence: a(i) = (i+2) * a(i-1) + sign * (i+1)!   */
        seq_t term = (seq_t)(i + 2) * a_cache[i - 2];
        int sign = ((i + 3) & 1) ? -1 : 1;   /* (-1)^(i+3) */
        if (sign == 1)
            term += last_fact;
        else
            term -= last_fact;   /* safe: term >= last_fact for all n ≥ 2 */

        ensure_a_capacity(a_cache_size + 1);
        a_cache[a_cache_size] = term;
        ++a_cache_size;
    }

    return a_cache[n - 1];
}
