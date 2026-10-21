/*
 * A222278  Number of nX5 0..5 arrays with no entry increasing mod 6 by 5
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1784*a(n-1) -372607*a(n-2) +28170251*a(n-3)
 *            -798596554*a(n-4) +1889917225*a(n-5) +177172468750*a(n-6)
 *            -651776500000*a(n-7) with a(1)=625, a(2)=972405,
 *            a(3)=1513010465, a(4)=2354171487645, a(5)=3662978221194885,
 *            a(6)=5699418948690910545, a(7)=8868023343395876315745
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"

static seq_t *cache = NULL;      /* array of computed a(n) values */
static size_t cache_cap = 0;     /* capacity of `cache` */
static size_t cache_len = 0;     /* number of entries currently stored */

/* Ensure that the cache can hold at least `need` elements */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise the first seven terms of the sequence.  This is done on
   the first call to `a`; subsequent calls reuse the stored values. */
static void init_initial_terms(void)
{
    if (cache_len >= 7)
        return;
    ensure_capacity(7);
    cache[0] = seq_from_str("625");
    cache[1] = seq_from_str("972405");
    cache[2] = seq_from_str("1513010465");
    cache[3] = seq_from_str("2354171487645");
    cache[4] = seq_from_str("3662978221194885");
    cache[5] = seq_from_str("5699418948690910545");
    cache[6] = seq_from_str("8868023343395876315745");
    cache_len = 7;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for n<1; return 0 */

    init_initial_terms();

    /* Return a stored value if already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Compute missing terms up to n */
    for (idx_t cur = cache_len + 1; cur <= n; ++cur) {
        /* Compute a(cur) using the recurrence, with terms
           indexed as cache[i] = a(i+1). */
        idx_t i1 = cur - 1;          /* index for a(cur) in cache */
        idx_t i2 = cur - 2;          /* a(cur-1) */
        idx_t i3 = cur - 3;          /* a(cur-2) */
        idx_t i4 = cur - 4;          /* a(cur-3) */
        idx_t i5 = cur - 5;          /* a(cur-4) */
        idx_t i6 = cur - 6;          /* a(cur-5) */
        idx_t i7 = cur - 7;          /* a(cur-6) */
        idx_t i8 = cur - 8;          /* a(cur-7) */

        seq_t pos = cache[i2] * (seq_t)1784
                  + cache[i4] * (seq_t)28170251
                  + cache[i6] * (seq_t)1889917225
                  + cache[i7] * (seq_t)177172468750;

        seq_t neg = cache[i3] * (seq_t)372607
                  + cache[i5] * (seq_t)798596554
                  + cache[i8] * (seq_t)651776500000;

        seq_t next = pos - neg; /* final term */

        ensure_capacity(i1 + 1);
        cache[i1] = next;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
