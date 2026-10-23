/*
 * A039179  Numbers whose base-11 representation has the same number of 2's
 *          and 8's.
 * https://oeis.org/A039179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth natural number whose base‑11 representation has the
 *            same number of 2's and 8's (OEIS A039179)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Forward declarations for static helpers */
static bool is_good(uint64_t x);

/* Static cache for memoisation */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t last_checked = 0;

/* Check whether a decimal integer x satisfies the condition. */
static bool is_good(uint64_t x)
{
    if (x == 0) return true;          /* 0 has no digits, counts are equal */
    int cnt2 = 0, cnt8 = 0;
    while (x) {
        int d = x % 11;
        if (d == 2) ++cnt2;
        else if (d == 8) ++cnt8;
        x /= 11;
    }
    return cnt2 == cnt8;
}

/* Return the n-th term (n ≥ 1).  Sequence is cached to allow arbitrary order
 * of calls.                                                      */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache     = seq_alloc(cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t x = last_checked;
        if (is_good(x)) {
            /* Grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t newcap = cache_cap * 2;
                seq_t *tmp    = seq_realloc(cache, cache_cap, newcap);
                if (tmp == NULL) /* allocation failed – abort gracefully */
                    break;
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = (seq_t)x;
        }
        ++last_checked;
    }

    return cache[(size_t)n - 1];
}
