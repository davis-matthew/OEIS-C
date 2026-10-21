/*
 * A211384  a(1) = 1, a(2) = 3; for n>2, a(n) = smallest number > a(n-1) such
 *          that a(n) is divisible by a(d) for all divisors d of n.
 * https://oeis.org/A211384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 117, u32 n <= 593, u64 n <= 2939,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211384: a(1)=1, a(2)=3; for n>2,
   a(n) = the smallest number > a(n‑1) divisible by a(d)
   for all divisors d of n. This implementation memoises
   values and uses LCMs to find the next term. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Cache for computed terms (1‑based). */
static seq_t *cache = NULL;
static size_t capacity = 0;      /* number of slots allocated (including unused 0) */
static size_t computed_max = 0;  /* largest index that has been computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;   /* slots from 0 .. n */
    if (needed <= capacity) return;

    size_t new_cap = capacity ? capacity * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* allocation failed */
    /* Zero new slots so uninitialised entries stay 0. */
    if (new_cap > capacity)
        memset(tmp + capacity, 0, (new_cap - capacity) * sizeof(seq_t));

    cache = tmp;
    capacity = new_cap;
}

/* Return the nth term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* unused */

    ensure_capacity(n);

    /* Return cached value if already computed. */
    if (n <= computed_max && cache[n] != (seq_t)0)
        return cache[n];

    /* Compute missing terms in increasing order. */
    for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
        if (i == 1) { cache[1] = (seq_t)1; continue; }
        if (i == 2) { cache[2] = (seq_t)3; continue; }

        seq_t lcm_val = (seq_t)1;   /* LCM of a(d) for all proper divisors d of i */

        for (idx_t d = 1; (size_t)d * (size_t)d <= i; ++d) {
            if (i % d == 0) {
                idx_t d1 = d;
                idx_t d2 = i / d;

                if (d1 < (idx_t)i) {
                    seq_t val = cache[d1];
                    seq_t g = gcd(lcm_val, val);
                    lcm_val = (lcm_val / g) * val;
                }
                if (d2 != d1 && d2 < (idx_t)i) {
                    seq_t val = cache[d2];
                    seq_t g = gcd(lcm_val, val);
                    lcm_val = (lcm_val / g) * val;
                }
            }
        }

        seq_t prev = cache[i - 1];
        seq_t multiplier = prev / lcm_val + (seq_t)1;
        cache[i] = multiplier * lcm_val;
    }

    computed_max = (size_t)n;
    return cache[n];
}
