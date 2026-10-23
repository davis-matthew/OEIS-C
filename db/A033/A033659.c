/*
 * A033659  Trajectory of 27 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 27; a(n) = a(n-1) + rev(a(n-1)) where rev(x) is the
 *            decimal reversal of x
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 18, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper: reverse the decimal digits of x. */
static seq_t reverse_digits(seq_t x)
{
    const seq_t TEN = (seq_t)10;
    seq_t rev = (seq_t)0;
    while (x != (seq_t)0) {
        seq_t digit = x % TEN;
        rev = rev * TEN + digit;
        x = x / TEN;
    }
    return rev;
}

/* Cached sequence values. */
static seq_t *cache      = NULL;
static size_t  cache_len = 0;   /* number of elements currently stored */
static size_t  cache_cap = 0;   /* allocated capacity */

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Expand the cache up to index n (if needed). */
    while (cache_len <= (size_t)n) {
        seq_t val;
        if (cache_len == 0) {
            val = (seq_t)27;
        } else {
            seq_t prev = cache[cache_len - 1];
            seq_t rev  = reverse_digits(prev);
            val = prev + rev;
        }

        /* Ensure sufficient storage. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed; terminate program (no other options). */
                abort();
            }
            cache      = tmp;
            cache_cap  = new_cap;
        }

        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[(size_t)n];
}
