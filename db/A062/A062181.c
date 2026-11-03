/*
 * A062181  Harmonic mean of digits is 3.
 * https://oeis.org/A062181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062181: Harmonic mean of digits is 3
 * a(n) = the n-th positive integer whose decimal digits have harmonic mean 3
 * (i.e. for a number with k digits d₁…d_k: k / Σ(1/dᵢ) = 3)
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a number has a harmonic mean of 3 */
static int is_hm3(seq_t x)
{
    if (x == (seq_t)0) return 0;          /* no positive digits */
    seq_t sum = 0;                        /* Σ 2520/dᵢ  as a seq_t */
    unsigned long long digits = 0;         /* count of digits */

    seq_t t = x;
    while (t != (seq_t)0) {
        int d = (int)(t % (seq_t)10);     /* current last digit */
        if (d == 0) return 0;             /* a zero digit disqualifies */
        sum += (seq_t)(2520 / d);          /* add 2520 divided by the digit */
        t   /= (seq_t)10;                 /* drop the digit */
        ++digits;
    }

    /* Condition: sum * 3 == 2520 * digits  (all a seq_t expressions) */
    if (sum * (seq_t)3 == (seq_t)2520 * (seq_t)digits)
        return 1;
    return 0;
}

/* Resize the cache array if more space is needed */
static seq_t *resize_cache(seq_t *ptr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = *cap ? (*cap << 1) : 4;
        while (new_cap < need) new_cap <<= 1;
        seq_t *np = mem_resize(seq_t, ptr, new_cap);
        if (!np) exit(EXIT_FAILURE);      /* allocation failure */
        *cap = new_cap;
        ptr = np;
    }
    return ptr;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* undefined for n = 0; return 0 just to keep a valid value */
    if (n == 0) return (seq_t)0;

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t cur = (seq_t)1;          /* next candidate to test */

    /* Grow cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        if (is_hm3(cur)) {
            cache = resize_cache(cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = cur;
        }
        ++cur;                            /* try next integer */
    }

    return cache[(size_t)n - 1];
}
