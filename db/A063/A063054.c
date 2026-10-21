/*
 * A063054  'Reverse and Add!' trajectory of 1997.
 * https://oeis.org/A063054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 32, u128 n <= 72,
 *            big any n
 * Helpers:   reverse_digits_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1997; a(n)=a(n-1)+reverse(a(n-1)) for n>0 */
#include <stdlib.h>
#include "../utils/reverse_digits_seq.h"

/* Static cache of computed terms. */
static seq_t *cache     = NULL;
static idx_t  cache_len = 0;   /* number of initialized entries */

/* Return the nth term of A063054. */
seq_t a(idx_t n)
{
    if (n < cache_len)                /* already computed */
        return cache[n];

    /* Ensure the array has space for indices 0..n. */
    size_t new_len = (size_t)n + 1;

    if (cache == NULL) {              /* first call */
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
        cache[0] = (seq_t)1997;
        cache_len = 1;
    } else if (new_len > cache_len) { /* need growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
    }

    /* Compute from the first missing index up to n. */
    idx_t i_start = cache_len;        /* old length before computation */
    for (idx_t i = i_start; i <= n; ++i) {
        seq_t rev = reverse_digits_seq(cache[i - 1]);
        cache[i] = cache[i - 1] + rev;
    }

    cache_len = new_len;              /* update the number of valid entries */
    return cache[n];
}
