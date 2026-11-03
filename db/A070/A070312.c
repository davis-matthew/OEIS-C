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
 * Validated: standard, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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
        seq_t mult        = pow10(num_digits(a_prev));
        seq_t product     = a_prevprev * mult;
        seq_t result      = a_prev + product + a_prev; /* 2*a_prev + product */
        cache[i - 1] = result;
    }
    cached_max = n;
    return cache[n - 1];
}
