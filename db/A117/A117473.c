/*
 * A117473  The values of 'a' in a^2 + b^2 = c^2, where b - a = 17 and gcd(a,
 *          b, c) = 1.
 * https://oeis.org/A117473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=7, a(2)=28, a(3)=88, a(4)=207, a(n)=6*a(n-2)-a(n-4)+34 for
 *            n>=5. Access: a(1) is the first term
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;      /* pointer to cached terms, cache[i] = a(i+1) */
static size_t cache_size = 0;    /* number of elements currently allocated */

/* Ensure that cache contains at least the first n terms (n>=1). */
static void ensure_cache(size_t n)
{
    if (n <= cache_size) return;           /* already enough */

    size_t new_size = cache_size > 0 ? cache_size << 1 : 4;
    if (new_size < n) new_size = n;

    /* realloc may return NULL on failure; abort as per the problem statement */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute any new entries */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i == 0)      cache[0] = (seq_t)7;
        else if (i == 1) cache[1] = (seq_t)28;
        else if (i == 2) cache[2] = (seq_t)88;
        else if (i == 3) cache[3] = (seq_t)207;
        else             cache[i] = (seq_t)6 * cache[i-2] - cache[i-4] + (seq_t)34;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* behaviour for index < 1 is irrelevant */

    /* idx is 1‑based; convert to 0‑based cache index */
    size_t idx = (size_t)n;
    assert(idx >= 1);                      /* enforce valid input */

    ensure_cache(idx);
    return cache[idx - 1];
}
