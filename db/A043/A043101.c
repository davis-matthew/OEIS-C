/*
 * A043101  Numbers k such that 0 and 2 occur juxtaposed in the base-4
 *          representation of k but not of k-1.
 * https://oeis.org/A043101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A043101: numbers k such that 0 and 2 occur juxtaposed in the
 * base‑4 representation of k but not of k‑1.  The function scans integers
 * and caches the results so that each request can be answered in O(1)
 * after the needed terms have been generated.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Return true if the base‑4 representation of v contains a pair of
 * adjacent digits 0 and 2 (in either order). */
static bool has_adjacent_0_2(idx_t v)
{
    if (v == 0) return false;
    int prev = v % 4;
    v /= 4;
    while (v > 0) {
        int cur = v % 4;
        if ((prev == 0 && cur == 2) || (prev == 2 && cur == 0))
            return true;
        prev = cur;
        v /= 4;
    }
    return false;
}

/* Cached sequence data. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;       /* number of terms already cached */
static size_t cache_cap  = 0;       /* capacity of cache array */
static idx_t  next_k     = 1;       /* next integer to test */

/* Ensure that cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Find next k that satisfies the definition. */
        for (;;) {
            if (has_adjacent_0_2(next_k) && !has_adjacent_0_2(next_k - 1)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);  /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_k;
                break;
            }
            ++next_k;
        }
        ++next_k;
    }
}

/* Return the n-th term (n>=1).  Behaviour for n<1 is unspecified. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, so this case is irrelevant */
    ensure_cache(n);
    return cache[n - 1];
}
