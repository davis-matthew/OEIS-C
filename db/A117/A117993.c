/*
 * A117993  Heptagonal numbers with only odd digits.
 * https://oeis.org/A117993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 93, u64 n <= 382, u128 n <= 382,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 368 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th heptagonal number H(k)=k*(5k-3)/2 that
   contains only odd decimal digits.  The function builds an internal
   cache so that calls in any order are cheap and the algorithm is
   O(n) with a modest constant. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

static bool has_only_odd_digits(seq_t x)
{
    if (x == 0)           /* 0 has an even digit */
        return false;
    while (x != 0) {
        seq_t d = x % 10;
        if ((d & 1u) == 0)   /* even digit found */
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    static idx_t last_k = 0;      /* largest k examined so far */
    static seq_t *cache = NULL;   /* array of previously found terms */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_size = 0; /* number of terms stored */

    if (n < 1)          /* undefined index */
        return (seq_t)0;

    /* Extend cache until it contains n terms */
    while (cache_size < n) {
        /* Search for next k giving an odd‑digit heptagonal number */
        do {
            ++last_k;
            seq_t kk = (seq_t)last_k;
            seq_t h = kk * (5 * kk - 3) / 2;
            if (has_only_odd_digits(h)) {
                if (cache_size + 1 > cache_cap)
                    ensure_capacity(cache_size + 1, &cache_cap,
                                    (void **)&cache, sizeof(seq_t));
                cache[cache_size++] = h;
                break;
            }
        } while (true);
    }

    return cache[n - 1];
}
