/*
 * A137469  Numbers with an odd number of 1's in base 5 expansion.
 * https://oeis.org/A137469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137469: a(n) = n-th integer whose base‑5 representation contains an odd number of digit 1s. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* return 1 if x has an odd number of 1 digits in base 5, else 0 */
static int odd_one_parity(idx_t x) {
    int p = 0;
    while (x) {
        if ((x % 5) == 1) p ^= 1;
        x /= 5;
    }
    return p;
}

/* cache for already computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_len = 0;
static idx_t  next_candidate = 1;   /* next number to test */

/* grow the cache to at least 'needed' elements */
static void grow_cache(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();                /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* required OEIS routine */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n < 1; offset is 1 */

    /* initialise cache on first call */
    if (!cache) {
        grow_cache(16);
        cache_len = 0;
        next_candidate = 1;
    }

    /* compute until we have at least n terms */
    while (cache_len < n) {
        idx_t val = next_candidate;
        while (!odd_one_parity(val)) ++val;   /* find next candidate */

        seq_t v = (seq_t)val;                /* store result */
        grow_cache(cache_len + 1);
        cache[cache_len++] = v;

        next_candidate = val + 1;            /* next search starts here */
    }

    return cache[n - 1];   /* a(1) is stored at index 0 */
}
