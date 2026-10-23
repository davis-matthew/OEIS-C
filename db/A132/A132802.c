/*
 * A132802  a(0)=1. a(n) = Sum_{k|n} a(floor(k/2)).
 * https://oeis.org/A132802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k|n} a(floor(k/2)), a(0)=1
 * Range:     u8 n <= 191, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoised integer‑sequence for OEIS A132802 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;   /* dynamic array of a(n) */
static idx_t  cap   = 0;      /* allocated size of cache */
static idx_t  comp  = 0;      /* largest index fully computed in cache */

/* Ensure that cache is at least |n|+1 elements and that a(0..n) are computed. */
static void ensure_cache(idx_t n)
{
    /* First time: allocate a modest initial capacity. */
    if (cache == NULL) {
        /* allocate at least 64 entries to avoid many reallocations */
        cap = 64;
        if (n+1 > cap) cap = n+1;
        cache = alloc_seq_array(cap);
        cache[0] = (seq_t)1;     /* a(0)=1 */
        comp  = 0;
    }

    /* Resize if needed. */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        if (new_cap < cap * 2) new_cap = cap * 2;
        seq_t *new_cache = alloc_seq_array(new_cap);

        /* copy old values (construction already handled by alloc functions). */
        for (idx_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];

        seq_free_array(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = comp + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d <= i / d; ++d) {          /* iterate divisors up to sqrt(i) */
            if (i % d == 0) {
                idx_t d1 = d;
                idx_t d2 = i / d;

                /* divisor d1 contributes a(floor(d1/2)) */
                sum += cache[d1 / 2];

                /* if d1 != d2 add the counterpart divisor */
                if (d1 != d2)
                    sum += cache[d2 / 2];
            }
        }
        cache[i] = sum;
    }
    comp = n;
}

/* Entry point required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
