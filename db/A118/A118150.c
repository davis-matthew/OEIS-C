/*
 * A118150  Start with 1 and repeatedly reverse the digits and add 53 to get
 *          the next term.
 * https://oeis.org/A118150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = reverse_decimal(a(n-1)) + 53, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                 /* for malloc/realloc, abort */
#include "../utils/reverse_digits.h"   /* seq_t reverse_digits(seq_t) */

static seq_t *seq_cache = NULL;      /* array of computed terms, 0‑based */
static idx_t  cache_cap  = 0;        /* capacity in seq_t elements */
static idx_t  cache_used = 0;        /* number of terms stored (>=0) */

/* Resize cache to hold at least n elements */
static void
ensure_cache_cap(idx_t n)
{
    if (n <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 1;
    while (newcap < (size_t)n)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* out of memory */
    seq_cache = tmp;
    cache_cap = (idx_t)newcap;
}

/* Compute a(n) using memoized recurrence */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                   /* offset is 1, but guard sanity */

    ensure_cache_cap(n);

    /* Initialize first term if not already */
    if (cache_used == 0) {
        seq_cache[0] = 1;            /* a(1) */
        cache_used   = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        seq_t prev = seq_cache[i - 2];          /* a(i-1) */
        seq_t rev  = reverse_digits(prev);      /* reversed digits */
        seq_cache[i - 1] = rev + (seq_t)53;      /* a(i) = rev + 53 */
    }
    cache_used = n; /* now we have all terms up to n */

    return seq_cache[n - 1]; /* 1‑based index */
}
