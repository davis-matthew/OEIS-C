/*
 * A037350  Numbers whose base-3 and base-9 expansions have no digits in
 *          common.
 * https://oeis.org/A037350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037350: Numbers whose base‑3 and base‑9 expansions have no digits in common.
   Equivalent characterization: the base‑9 expansion uses only digits 3‑8. */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache      = NULL;   /* cache of found terms */
static idx_t  cache_cap  = 0;      /* allocated capacity of cache */
static idx_t  cache_len  = 0;      /* number of terms stored */
static seq_t  last_term  = 0;      /* last term found (for next start) */

/* ensure that `cache` has at least `min_cap` slots, reallocate if necessary   */
static void ensure_cache_cap(idx_t min_cap)
{
    if (min_cap <= cache_cap)
        return;

    idx_t new_cap = cache_cap * 2;
    if (new_cap < min_cap)
        new_cap = min_cap;

#ifdef __cplusplus
    /* C++: use operator new/delete to construct & destruct objects */
    seq_t *new_arr = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];
    delete[] cache;
#else
    /* C: use realloc which merely copies bytes */
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache      = new_arr;
    cache_cap  = new_cap;
}

/* test whether `x` has a base‑9 expansion that uses only digits 3‑8          */
static bool no_common_digits(seq_t x)
{
    seq_t t = x;
    while (t > 0) {
        unsigned d = (unsigned)(t % 9U);
        if (d < 3U)
            return false;           /* a forbidden digit 0,1,2 appears */
        t /= 9;
    }
    return true;
}

/* Return the n‑th term (indexing starts at 1).                                 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* lower indices are undefined */

    /* initialise cache on first call                                    */
    if (!cache) {
        cache_cap = 64;              /* arbitrary start size */
        cache_len = 0;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
    }

    /* generate terms until we have at least n of them                    */
    seq_t cur = (last_term == 0) ? 3 : last_term + 1;
    while (cache_len < n) {
        if (no_common_digits(cur)) {
            if (cache_len == cache_cap)
                ensure_cache_cap(cache_cap * 2);
            cache[cache_len++] = cur;
            last_term = cur;
        }
        ++cur;
    }

    return cache[n - 1];
}
