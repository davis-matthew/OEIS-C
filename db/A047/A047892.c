/*
 * A047892  a(1) = 2; for n > 0, a(n+1) = a(n) * sum of digits of a(n).
 * https://oeis.org/A047892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A047892
   a(1) = 2;  for n > 0, a(n+1) = a(n) * sum_of_digits(a(n))   */

/* Utility: digit_sum is provided in ../utils/digit_sum.h.
   It returns the sum of the decimal digits of its argument.      */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"   /* seq_t digit_sum(seq_t v) */

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static idx_t  cap   = 0;      /* allocated capacity (>= cache_len+1) */
static idx_t  len   = 0;      /* number of terms currently stored (>=1) */

/* Ensure the cache holds at least indices up to n (inclusive).        */
static void
ensure_cache_capacity(idx_t n)
{
    if (cap >= n)
        return;                               /* already sufficient */

    idx_t new_cap = (cap > 0) ? cap : 1;
    while (new_cap < n)
        new_cap <<= 1;                         /* grow exponentially */

    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                               /* malloc failure */
    cache = tmp;
    cap   = new_cap;
}

/* Main sequence function.  It accepts any n ≥ 1 in arbitrary order. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                       /* undefined, harmless */

    /* Allocate space for the requested term if necessary. */
    ensure_cache_capacity(n);

    /* Initialise the first term on the very first call. */
    if (len == 0) {
        cache[1] = (seq_t)2;
        len = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t sum  = digit_sum(prev);
        cache[i]   = prev * sum;
    }
    len = n; /* now we have terms up to n stored */

    return cache[n];
}
