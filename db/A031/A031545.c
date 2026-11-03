/*
 * A031545  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 47.
 * https://oeis.org/A031545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by search: iterate over k until n terms satisfying the
   continued‑fraction property are found. */

#include <stdlib.h>
#include <stdint.h>
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

static int
is_candidate(seq_t k)
{
    seq_t a0 = floor_sqrt(k);
    if (a0 == 0)
        return 0;
    /* Exclude perfect squares. */
    if ((k % a0 == 0) && (k / a0 == a0))
        return 0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    /* Dynamic array to store the periodic part. */
    static seq_t *period = NULL;
    static size_t period_cap = 0;
    if (period == NULL) {
        period_cap = 1024;
        period = mem_new(seq_t, period_cap);
    }

    size_t len = 0;
    do {
        m = d * a - m;
        seq_t m2 = m * m;
        d = (k - m2) / d;
        a = (a0 + m) / d;

        if (len == period_cap) {
            period_cap <<= 1; /* double capacity */
            period = mem_resize(seq_t, period, period_cap);
        }
        period[len++] = a;
    } while (a != 2 * a0);

    /* The period must be even. */
    if (len & 1)
        return 0;

    /* After deleting the last term (2*a0) the remaining length is odd,
       so the central index is (len-1)/2. */
    size_t mid = (len - 1) / 2;
    return (period[mid] == 47);
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* Simple cache of discovered terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_k = 0;

    size_t target = (size_t)n;
    if (target <= cache_len)
        return cache[target - 1];

    /* Extend cache until it contains at least target terms. */
    while (cache_len < target) {
        ++last_k; /* next integer to test */
        if (is_candidate(last_k)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newptr = mem_resize(seq_t, cache, new_cap);
                if (!newptr)
                    exit(1); /* allocation failure */
                cache = newptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_k;
        }
    }
    return cache[target - 1];
}
