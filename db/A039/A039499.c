/*
 * A039499  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 7's.
 * https://oeis.org/A039499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑12 representation has the same nonzero number
 *            of 0’s and 7’s
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;     /* cached sequence terms             */
static size_t cache_cap = 0;    /* allocated capacity of cache       */
static size_t cached_terms = 0; /* number of terms currently stored  */
static seq_t next_scan = 0;     /* next integer to test             */

/* Ensure cache has room for at least 'need' terms. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

/* Test whether a number in base‑12 has an equal non‑zero number of
   0’s and 7’s. */
static bool qualifies(seq_t x)
{
    int cnt0 = 0, cnt7 = 0;
    seq_t y = x;
    while (y > 0) {
        seq_t d = y % 12;
        if (d == 0) ++cnt0;
        else if (d == 7) ++cnt7;
        y /= 12;
    }
    return cnt0 == cnt7 && cnt0 > 0;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* generate terms until we have at least 'n' terms stored */
    while (cached_terms < n) {
        /* find next qualifying number */
        do {
            ++next_scan;
        } while (!qualifies(next_scan));

        ensure_capacity(cached_terms + 1);
        cache[cached_terms++] = next_scan;
    }
    return cache[n - 1];
}
