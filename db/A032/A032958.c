/*
 * A032958  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8804 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032958: numbers whose base‑8 representation has even
   digits at all odd positions (counting from the most significant digit).
   The function generates the sequence incrementally, caching results
   to support nondeterministic order of calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if every digit at an odd position (counted from the
   most significant digit, i.e. positions 1,3,5,…) of the base‑8
   expansion of x is even. */
static bool valid_oct_evens(seq_t x)
{
    /* At most 128 octal digits are needed for all supported seq_t widths. */
    uint8_t dig[128];
    size_t len = 0;
    seq_t t = x;

    /* Extract digits in reverse order: dig[0] is least significant. */
    do {
        dig[len++] = (uint8_t)(t % 8);
        t /= 8;
    } while (t != 0);

    /* Check odd positions from the left. */
    for (size_t i = 0; i < len; ++i) {
        size_t pos_from_left = len - 1 - i;   /* 0‑based index from left */
        if (pos_from_left & 1U) {             /* odd position? (1,3,5,…) */
            if (dig[i] & 1U)                  /* digit odd? */
                return false;
        }
    }
    return true;
}

/* Cache for previously computed sequence values. */
static seq_t *cache = NULL;
static idx_t cached_cnt = 0;     /* number of terms currently cached */
static size_t cache_cap = 0;     /* capacity of the cache */

static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(1);                 /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Next candidate value to test after the last value examined. */
static seq_t next_candidate = 1;

/* Main function requested by the interface. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined; return 0 for safety */
        return (seq_t)0;

    /* Make sure the cache has at least n slots. */
    ensure_cache_capacity((size_t)n);

    /* Generate new terms until we have at least n of them. */
    while (cached_cnt < n) {
        if (valid_oct_evens(next_candidate))
            cache[cached_cnt++] = next_candidate;
        ++next_candidate;   /* advance candidate regardless of validity */
    }

    return cache[n - 1];
}
