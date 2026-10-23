/*
 * A164396  Number of binary strings of length n with no substrings equal to
 *          0001 or 0110.
 * https://oeis.org/A164396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 82, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164396: Number of binary strings of length n with no substrings
 * equal to 0001 or 0110.
 *
 * Recurrence: a(n) = 2*a(n-1) - a(n-3) + a(n-5) - a(n-6)
 * with initial values a(0)=1, a(1)=2, a(2)=4, a(3)=8,
 * a(4)=14, a(5)=24.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms               */
static size_t cache_size = 0; /* number of terms currently stored (≥ 0) */

/* Return a(n).  The function memoises results so that subsequent
 * calls (in any order) are fast. */
seq_t a(idx_t n)
{
    /* Initialise the first 6 terms on the first call. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)2;          /* a(1) */
        cache[2] = (seq_t)4;          /* a(2) */
        cache[3] = (seq_t)8;          /* a(3) */
        cache[4] = (seq_t)14;         /* a(4) */
        cache[5] = (seq_t)24;         /* a(5) */
        cache_size = 6;               /* we have terms 0..5 */
    }

    /* If the requested index is already known, return it immediately. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow the cache to hold at least n+1 terms.  Double the capacity
     * to keep the amortised cost linear. */
    size_t new_cap = cache_size;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure */
    cache = tmp;

    /* Compute the missing terms, starting from the first unknown index
     * (which is at least 6, because the first 6 terms are already set). */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t val = cache[(size_t)(i - 1)];   /*  a(i-1)   */
        val += val;                          /* 2*a(i-1)  */
        val -= cache[(size_t)(i - 3)];        /* -a(i-3)   */
        val += cache[(size_t)(i - 5)];        /* +a(i-5)   */
        val -= cache[(size_t)(i - 6)];        /* -a(i-6)   */
        cache[(size_t)i] = val;
    }

    cache_size = (size_t)n + 1;         /* update stored count */
    return cache[(size_t)n];
}
