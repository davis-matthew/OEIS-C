/*
 * A070312  a(1) = a(2) = 1; a(n) = a(n-1) + concatenation of a(n-2) and
 *          a(n-1).
 * https://oeis.org/A070312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = a(2) = 1; a(n) = a(n-1) + concat(a(n-2), a(n-1)) with
 *            concat(x,y)=x*10^digits(y)+y
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, ensure_cache, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_max = 0;   /* number of terms already computed */

/* Return the n-th term of A070312 (1‑based index). */
seq_t a(idx_t n)
{
    /* Allocate or enlarge the cache if needed */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise base terms if they are not yet in the cache */
    if (cached_max < 1 && n >= 1) {
        cache[0] = (seq_t)1;
        cached_max = 1;
    }
    if (cached_max < 2 && n >= 2) {
        cache[1] = (seq_t)1;
        cached_max = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t a_prev      = cache[i - 2];   /* a(n-1) */
        seq_t a_prevprev  = cache[i - 3];   /* a(n-2) */
        seq_t mult        = pow10(digit_len(a_prev));
        seq_t product     = a_prevprev * mult;
        seq_t result      = a_prev + product + a_prev; /* 2*a_prev + product */
        cache[i - 1] = result;
    }
    cached_max = n;
    return cache[n - 1];
}
