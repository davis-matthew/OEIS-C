/*
 * A072452  a(n) = reversal(a(n-1)+n) for n>0, a(0) = 0.
 * https://oeis.org/A072452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 83, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   revseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0
   a(n) = reverse_digits(a(n-1) + n) for n > 0          */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/revseq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated capacity in elements */
    static size_t   len   = 0;   /* number of elements already stored (max index + 1) */

    /* initialize on first call */
    if (len == 0) {
        cap = 1;
        len = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failed */
        cache[0] = (seq_t)0;                  /* a(0) = 0 */
    }

    /* if already computed, just return */
    if (n < len) return cache[n];

    /* grow cache to hold up to index n */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap << 1 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* reallocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        seq_t sum = cache[i - 1] + (seq_t)i;
        cache[i]  = revseq(sum);
    }
    len = n + 1;

    return cache[n];
}
