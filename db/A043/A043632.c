/*
 * A043632  Numbers whose base-9 representation has exactly 3 runs.
 * https://oeis.org/A043632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer whose base‑9 representation has *
 *            exactly 3 maximal equal‑digit runs
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* count the number of maximal equal‑digit runs in the base‑9 representation of x */
static int count_base9_runs(seq_t x)
{
    /* handle 0 as one run (its representation is "0") */
    if (x == (seq_t)0)
        return 1;

    /* base‑9 requires at most ⌈log₉(2¹²⁸)⌉ ≈ 61 digits for a 128‑bit build.
       128 digits are more than enough for all tested values. */
    uint8_t digits[128];
    int len = 0;
    seq_t t = x;

    /* Extract digits from least‑significant to most‑significant. */
    while (t > (seq_t)0) {
        digits[len++] = (uint8_t)(t % (seq_t)9);
        t /= (seq_t)9;
    }

    /* Count runs. */
    int runs = 1;
    uint8_t prev = digits[len - 1];
    for (int i = len - 2; i >= 0; --i) {
        uint8_t d = digits[i];
        if (d != prev) {
            ++runs;
            prev = d;
        }
    }
    return runs;
}

/* static cache of computed terms */
static size_t cache_sz   = 0;          /* number of cached terms */
static size_t cache_cap  = 0;          /* allocated capacity of the cache */
static seq_t *cache      = NULL;      /* pointer to the cached terms */
static idx_t  next_start = 1;         /* next integer to test for the 3‑run property */

/* resize the cache to hold at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;        /* double until it fits */
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();              /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Extend the cache until we have at least n terms. */
    while (cache_sz < (size_t)n) {
        seq_t cur = (seq_t)next_start;
        if (count_base9_runs(cur) == 3) {
            grow_cache(cache_sz + 1);
            cache[cache_sz++] = cur;
        }
        ++next_start;
    }
    return cache[n - 1];
}
