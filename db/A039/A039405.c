/*
 * A039405  Numbers whose base-10 representation has the same nonzero number
 *          of 1's and 5's.
 * https://oeis.org/A039405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: the n-th positive integer whose decimal representation
   contains the same non‑zero number of 1’s and 5’s.  The function
   generates the sequence by scanning increasing integers. */
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

static inline bool is_good(seq_t x) {
    unsigned cnt1 = 0, cnt5 = 0;
    seq_t t = x;
    while (t > 0) {
        unsigned d = (unsigned)(t % 10);
        if (d == 1) ++cnt1;
        else if (d == 5) ++cnt5;
        t = t / 10;
    }
    return cnt1 == cnt5 && cnt1 > 0;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static seq_t current = 0;
    static bool initialized = false;

    if (!initialized) {   /* one‑time initialization */
        initialized = true;
        cache = NULL;
        cache_size = 0;
        cache_cap = 0;
        current = 0;
    }

    size_t target;
    if (n == 0) return 0;   /* undefined for n<1, return 0 as a guard */

    target = (size_t)n;
    if (target <= cache_size)
        return cache[target - 1];

    while (cache_size < target) {
        ++current;                     /* skip 0 and start from 1 */
        if (is_good(current)) {
            if (cache_size == cache_cap)
                ensure_capacity(cache_size + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = current;
        }
    }
    return cache[target - 1];
}
