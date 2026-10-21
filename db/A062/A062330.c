/*
 * A062330  a(n) = product of the sum and product of the digits of a(n-1) (0
 *          is not to be considered a factor in the product).
 * https://oeis.org/A062330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (sum of digits of a(n-1)) · (product of non‑zero digits
 *            of a(n-1)) with a(1) = 2 The implementation memoises the
 *            sequence; it works for any seq_t that supports +, -, *, /, % and
 *            the assignment operator
 * Range:     u8 n <= 5, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

static seq_t digit_sum(seq_t x) {
    seq_t s = 0;
    while (x != 0) {
        seq_t d = x % 10;
        s += d;
        x /= 10;
    }
    return s;
}

static seq_t digit_prod_nonzero(seq_t x) {
    seq_t p = 1;
    while (x != 0) {
        seq_t d = x % 10;
        if (d != 0)
            p *= d;
        x /= 10;
    }
    return p;
}

/* Memoisation storage */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_len = 0;      /* number of valid terms stored (cache[0] = a(1)) */
static size_t cache_cap = 0;      /* current allocation capacity */

/* Grow the cache capacity to hold at least new_cap elements. */
static void ensure_cap(size_t new_cap) {
    if (new_cap <= cache_cap)
        return;
    size_t new_cap2 = new_cap < 8 ? 8 : new_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap2 * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – in a full implementation this would signal an
           error; here we simply abort. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap2;
}

/* Sequence function, external linkage as requested. */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* Initialise the first term a(1)=2 if not yet cached. */
    if (cache_len == 0) {
        ensure_cap(1);
        cache[0] = 2;
        cache_len = 1;
    }

    /* Return cached value if available. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms up to a(n). */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t s = digit_sum(prev);
        seq_t p = digit_prod_nonzero(prev);
        seq_t next = s * p;
        ensure_cap(i + 1);
        cache[i] = next;
    }
    cache_len = n;
    return cache[n - 1];
}
