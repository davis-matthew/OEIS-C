/*
 * A044502  Numbers n such that string 3,1 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044502: a(n) is the nth integer whose base‑6 representation
   contains the substring 31 but n+1 does not.  The function searches
   sequentially and memoises results, so any order of calls is handled. */

#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>  /* bool */
#include "../utils/mem.h"

static bool has31_base6(seq_t x)
{
    /* Return true iff the base‑6 representation of x contains the
       consecutive digits 3 followed by 1. */
    if (x == (seq_t)0)
        return false;
    uint8_t dig[64];   /* Max 64 digits for 128‑bit numbers */
    size_t len = 0;
    while (x != (seq_t)0) {
        dig[len++] = (uint8_t)(x % (seq_t)6);
        x = x / (seq_t)6;
    }
    /* Scan from most significant to least. */
    for (size_t i = len; i-- > 1; ) {
        if (dig[i] == 3U && dig[i - 1] == 1U)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Memoised sequential construction. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t n_terms = 0;       /* number already stored (a(1) at cache[0]) */
    static seq_t next_candidate = 1;/* next integer to test */

    if (n == 0)
        return (seq_t)0;           /* undefined, but no harm */

    /* ensure cache capacity */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < n)
            new_cap <<= 1;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }

    /* generate terms until we have n terms */
    while (n_terms < n) {
        seq_t m = next_candidate++;
        if (has31_base6(m) && !has31_base6(m + (seq_t)1))
            cache[n_terms++] = m;
    }

    return cache[n - 1];
}
