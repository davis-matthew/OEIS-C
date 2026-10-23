/*
 * A039330  Numbers whose base-8 representation has the same nonzero number of
 *          0's and 2's.
 * https://oeis.org/A039330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑8 representation has the same nonzero number of 0's
   and 2's.  The function returns the n-th such number (a(1)=16).  It
   builds the sequence on demand, caching already found terms so that
   any order of calls is fast. */
#include <stdlib.h>

/* Cache for already‑generated values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_test = 1;          /* next candidate to examine */

/* Grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_cap2 = cache_cap == 0 ? 128 : cache_cap;
    while (new_cap2 < new_cap)
        new_cap2 <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap2 * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory */
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    free(cache);
    cache = tmp;
    cache_cap = new_cap2;
}

/* Generate the next suitable number in order. */
static seq_t next_term(void)
{
    seq_t val = next_test;
    for (;; ++val) {
        idx_t cnt0 = 0, cnt2 = 0;
        seq_t tmp = val;
        /* Count digits in base 8.  Handle zero specially. */
        if (tmp == 0) {
            cnt0 = 1;
        } else {
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 8);
                if (d == 0)
                    ++cnt0;
                else if (d == 2)
                    ++cnt2;
                tmp = tmp / 8;
            }
        }
        if (cnt0 == cnt2 && cnt0 > 0) {
            next_test = val + 1;
            return val;
        }
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1; undefined for n<1 */
    /* Ensure cache contains at least n values. */
    while (cache_len < n) {
        grow_cache(cache_len + 64);
        cache[cache_len++] = next_term();
    }
    return cache[n - 1];
}
