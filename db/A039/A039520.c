/*
 * A039520  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 9's.
 * https://oeis.org/A039520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Sequential search for numbers whose base‑12 representation
   contains equal non‑zero counts of digits 2 and 9. */
#include <stdbool.h>
#include <stddef.h>

/* Return true if x in base‑12 has the same non‑zero number of 2’s and 9’s. */
static bool has_equal_2_9(idx_t x)
{
    size_t c2 = 0, c9 = 0;
    while (x > 0) {
        int d = (int)(x % 12);
        if (d == 2) {
            c2++;
        } else if (d == 9) {
            c9++;
        }
        x /= 12;
    }
    return (c2 == c9) && (c2 > 0);
}

/* Maximum number of terms we cache – large enough for n ≤ 10 000. */
#define MAX_CACHED 200000

static seq_t cache[MAX_CACHED];   /* cached values */
static size_t cached_cnt = 0;     /* number of values stored */
static idx_t next_candidate = 1;  /* next integer to test */

/* Append a value to the cache (assumes cache has space). */
static void cache_push(seq_t v)
{
    cache[cached_cnt++] = v;
}

/* a(n) – OEIS A039520 – n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)            /* behaviour below the offset is irrelevant   */
        return 0;

    /* Generate terms until we have at least n cached. */
    while (cached_cnt < n) {
        /* Find next integer satisfying the property. */
        while (!has_equal_2_9(next_candidate))
            next_candidate++;
        if (cached_cnt < MAX_CACHED)   /* safety check – should never hit */
            cache_push((seq_t)next_candidate);
        next_candidate++;
    }

    return cache[n - 1];
}
