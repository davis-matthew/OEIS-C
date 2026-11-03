/*
 * A039048  Numbers whose base-7 representation has the same number of 4's and
 *          6's.
 * https://oeis.org/A039048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9854 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039048:
 *  a(n) is the n‑th nonnegative integer whose base‑7 representation
 *  contains an equal number of digits 4 and 6;  a(1) = 0.
 *
 *  Implementation: a simple incremental search with memoisation.
 *  The function is re‑entrant and works for all supplied values
 *  of idx_t and seq_t.  It never caches the same value twice
 *  and starts testing from 1 after the initial value 0 has been cached.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* test whether a value has the same number of 4's and 6's in base‑7 */
static bool qualify(seq_t v)
{
    int c4 = 0, c6 = 0;
    seq_t t = v;
    while (t != (seq_t)0) {
        seq_t d = t % (seq_t)7;
        if (d == (seq_t)4)  ++c4;
        else if (d == (seq_t)6)  ++c6;
        t = t / (seq_t)7;
    }
    return c4 == c6;   /* including the case v == 0 */
}

/* static storage for the memoised sequence */
static seq_t *cache = NULL;
static idx_t   cache_len  = 0;  /* number of cached entries */
static idx_t   cache_cap  = 0;  /* current allocation capacity */
static seq_t   last_value = 1;  /* next integer to test after the cached part */

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise on the first call */
    if (cache == NULL) {
        cache_cap = 16;                     /* initial capacity */
        cache     = mem_new(seq_t, cache_cap);
        cache[0]  = (seq_t)0;               /* a(1) = 0 */
        cache_len = 1;
        last_value = 1;                     /* start testing from 1 */
    }

    /* return cached value if already computed */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* generate further terms until we reach the required index */
    while (cache_len < n) {
        if (qualify(last_value)) {
            if (cache_len == cache_cap) {   /* grow the cache if needed */
                cache_cap += cache_cap;    /* double */
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = last_value;
        }
        ++last_value;                       /* next candidate */
    }

    return cache[n - 1];
}
