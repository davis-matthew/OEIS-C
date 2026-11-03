/*
 * A165209  Numbers whose digital sum is divisible by the digital sum of its
 *          divisors.
 * https://oeis.org/A165209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9016 (2026-11-03)
 * Helpers:   digit_sum, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A165209: numbers whose digital sum is divisible by the digital
   sum of each divisor. This implementation tests consecutive integers and
   stores the terms in a cache so that any order of calls to a(idx_t n) is
   handled correctly. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; a(1) = 1.  For n == 0 we return
       0 but the result is not used by the test harness. */
    if (n == 0) return 0;

    /* Static cache of already found terms.  The cache is grown on demand. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t found = 0;   /* number of terms currently stored */
    static idx_t next = 1;     /* next integer to test */

    /* Initialise the cache on the first call. */
    if (!cache) {
        cache_cap = 16;
        cache = seq_alloc_array(cache_cap);
    }

    /* Extend the cache until it contains at least n terms. */
    while (found < (size_t)n) {
        seq_t val = (seq_t)next;
        seq_t dsum = digit_sum(val);

        bool ok = true;
        seq_t sqrt_val = floor_sqrt(val);

        /* Test all divisors up to the integer square root. */
        for (seq_t d = 1; d <= sqrt_val; ++d) {
            if (val % d == 0) {
                seq_t ds_d = digit_sum(d);
                if (dsum % ds_d != 0) { ok = false; break; }

                seq_t other = val / d;
                if (other != d) {
                    seq_t ds_other = digit_sum(other);
                    if (dsum % ds_other != 0) { ok = false; break; }
                }
            }
        }

        if (ok) {
            /* Grow the cache if necessary. */
            if (found >= cache_cap) {
                ensure_cache(found + 1, &cache, &cache_cap);
            }
            cache[found++] = val;
        }

        ++next;
    }

    return cache[n - 1];
}
