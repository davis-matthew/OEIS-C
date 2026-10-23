/*
 * A039473  Numbers whose base-11 representation has the same nonzero number
 *          of 4's and 6's.
 * https://oeis.org/A039473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n-th positive integer whose base‑11 representation
 *  contains an equal, non‑zero number of digits 4 and 6.   */

#include <stdlib.h>

static seq_t      *cache   = NULL;  /* table of found terms   */
static size_t      cache_len = 0;   /* how many terms stored */
static size_t      cache_cap = 0;   /* allocated capacity    */
static seq_t        last_checked = 0;   /* last value examined   */

static void grow_cache(void)
{
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

static int good_value(seq_t x)
{
    unsigned cnt4 = 0, cnt6 = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 11);
        if (d == 4) ++cnt4;
        else if (d == 6) ++cnt6;
        x /= 11;
    }
    return cnt4 > 0 && cnt4 == cnt6;
}

static void extend_cache(idx_t needed)
{
    while (cache_len < (size_t)needed) {
        seq_t val = last_checked + 1;
        if (good_value(val)) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = val;
        }
        last_checked = val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but harmless */
    extend_cache((size_t)n);
    return cache[n - 1];
}
