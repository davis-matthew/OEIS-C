/*
 * A138563  Beastly fax numbers: numbers containing the fax number of the
 *          Beast (667, one more than its regular number) in their decimal
 *          expansion.
 * https://oeis.org/A138563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, dumb, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 982 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Beastly fax numbers: numbers containing the decimal
   substring "667".  The function generates terms by brute‑force
   scanning the natural numbers and caching results so that calls
   in any order are handled efficiently. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff x contains the consecutive digits 6,6,7 in decimal. */
static bool has_sub667(seq_t x)
{
    if (x == 0)
        return false;
    seq_t prev1 = 10;   /* sentinel values > 9 */
    seq_t prev2 = 10;
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 6 && prev1 == 6 && prev2 == 7)
            return true;
        prev2 = prev1;
        prev1 = d;
        x /= 10;
    }
    return false;
}

static seq_t *cache = NULL;        /* cached terms */
static size_t cache_cap = 0;       /* allocated size of cache   */
static size_t cache_len = 0;       /* number of terms stored   */
static seq_t last_checked = 0;     /* last integer examined   */

/* Grow the cache so that it holds at least n terms. */
static void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* Allocate or grow the buffer as needed. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = mem_resize(seq_t, cache, cache_cap);
    }

    seq_t cur = last_checked + 1;
    while (cache_len < (size_t)n) {
        if (has_sub667(cur)) {
            cache[cache_len++] = cur;
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
        }
        cur++;
    }
    last_checked = cur - 1;
}

/* Public interface: return the nth Beastly fax number. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for 0 */
    ensure_terms(n);
    return cache[n - 1];
}
