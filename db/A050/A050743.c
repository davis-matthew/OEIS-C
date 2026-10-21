/*
 * A050743  Numbers k such that the decimal expansion of k^4 contains no pair
 *          of consecutive equal digits.
 * https://oeis.org/A050743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A050743:
   a(n) = the nth integer k (1‑indexed) such that the decimal expansion of k⁴
   contains no pair of consecutive equal digits. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true if the 128‑bit unsigned value v has no consecutive equal
   decimal digits. 0 is accepted. */
static bool no_consecutive_equal_digits(unsigned __int128 v)
{
    if (v == 0) return true;
    int prev = 10;                 /* impossible digit */
    while (v > 0) {
        int d = (int)(v % 10);
        if (d == prev) return false;
        prev = d;
        v /= 10;
    }
    return true;
}

/* Grow the cache array [cache] so it can hold at least [need] elements. */
static void ensure_cache_cap(size_t need, seq_t **cache, size_t *cap)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < need) new_cap = need;
        *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* The sequence function.
   It uses a static dynamic array (cache) that stores the successive terms
   found.  [next_candidate] remembers where the search left off, allowing
   subsequent calls to resume efficiently. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of found terms */
    static size_t cache_len = 0;    /* number of stored terms */
    static size_t cache_cap = 0;    /* allocated capacity */
    static uint64_t next_candidate = 0;   /* next k to test */

    if (n == 0) return 0;          /* undefined for index 0 */

    /* Grow cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Test candidate k = next_candidate. */
        unsigned __int128 v = (unsigned __int128)next_candidate;
        v *= next_candidate;
        v *= next_candidate;
        v *= next_candidate;      /* compute k⁴ */

        if (no_consecutive_equal_digits(v)) {
            /* Store k as seq_t. */
            ensure_cache_cap(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;         /* advance to next integer */
    }

    return cache[(size_t)n - 1];
}
