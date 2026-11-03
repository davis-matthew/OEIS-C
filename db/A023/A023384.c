/*
 * A023384  a(n+1) = a(n) converted to base 7 from base 6 (written in base
 *          10).
 * https://oeis.org/A023384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n) converted from base‑6 to base‑7 (in base‑10); a(1)
 *            = 6 (OEIS A023384)
 * Range:     u8 n <= 21, u16 n <= 31, u32 n <= 40, u64 n <= 49, u128 n <= 58,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 95 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free(seq_t *p) { mem_free(p); }

/* Convert a value written in base‑6 into its value in base‑7.
   The argument and return type are seq_t; all arithmetic is performed
   with the seq_t operators.  The function does not modify the input. */
static seq_t conv_base6_to_base7(seq_t x)
{
    /* Guaranteed to be long enough for all sizes encountered up to n ~ 10 000
       (the number of base‑6 digits grows roughly linearly with n). */
    uint8_t digits[20000];
    size_t dlen = 0;

    /* Extract base‑6 digits (least significant first). */
    while (x > 0) {
        seq_t q = x / 6;
        seq_t r = x % 6;
        digits[dlen++] = (uint8_t)r;
        x = q;
    }

    if (dlen == 0)     /* x was initially 0 */
        return 0;

    /* Reconstruct the value in base‑7. */
    seq_t res = 0;
    for (size_t i = dlen; i-- > 0; ) {
        res = res * 7 + digits[i];
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Cache the sequence values so that repeated calls are fast and order
       of calls does not matter.  The cache is allocated lazily. */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;   /* allocated capacity (in terms) */

    if (n == 0)          /* behaviour for n < 1 is undefined by the OEIS
                            offset; returning 0 is harmless. */
        return 0;

    /* Initialise the cache with the first term a(1) = 6. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = seq_alloc_new_array(cache_cap);
        cache[0] = 6;  /* a(1) = 6 */
        cache_len = 1;
    }

    /* Extend the cache up to the requested index n. */
    while (cache_len < (size_t)n) {
        seq_t next = conv_base6_to_base7(cache[cache_len - 1]);

        /* Grow the array if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *new_cache = seq_alloc_new_array(new_cap);
            /* Copy existing values. */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            /* Free the old array (all seq_t objects are destructed by seq_free). */
            seq_free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
