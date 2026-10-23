/*
 * A173646  Fixed points of the operation x->(max_d(x)+min_d(x))/2, where
 *          max_d(x) iteratively replaces each digit d of x from left to right
 *          with the largest of the d digits to its right (itself included),
 *          and similarly for min_d.
 * https://oeis.org/A173646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer x ≥1 satisfying x = (min_d(x)+max_d(x))/2 ;
 *            min_d and max_d are computed from the decimal digits of x, the
 *            operation is undefined for a digit 0, so such numbers are never
 *            considered fixed points
 * Range:     u8 n <= 36, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   minmax_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/minmax_d.h"

/* Return true iff x contains a decimal digit 0. */
static bool has_zero(seq_t x)
{
    if (x == 0) return false;           /* 0 itself is not in the sequence */
    while (x != 0) {
        if (x % 10 == 0) return true;
        x /= 10;
    }
    return false;
}

/* Cache for computed fixed points */
static seq_t *cache   = NULL;
static idx_t cache_len   = 0;
static idx_t cache_cap   = 0;
static seq_t next_test = 1;

static void ensure_cache(idx_t required)
{
    if (required <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < required) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for the build */

    /* Return cached value if available. */
    if (n <= cache_len) return cache[n - 1];

    ensure_cache(n);
    while (cache_len < n) {
        seq_t xi = next_test++;
        if (has_zero(xi)) continue;   /* skip numbers containing 0 */

        seq_t mind, maxd;
        minmax_d(xi, &mind, &maxd);
        seq_t sum = mind + maxd;
        if ((sum & 1) != 0) continue;   /* sum must be even */
        seq_t half = sum >> 1;
        if (half == xi) {
            cache[cache_len++] = xi;
        }
    }
    return cache[n - 1];
}
