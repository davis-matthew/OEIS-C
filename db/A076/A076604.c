/*
 * A076604  a(1) = 11, a(n_even) = (a(n-1)^2-1)/2; a(n_odd) = a(n-1)+1.
 * https://oeis.org/A076604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076604
   a(1) = 11
   a(n_even) = (a(n-1)^2 - 1) / 2
   a(n_odd)  = a(n-1) + 1
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache of computed terms */
    static size_t   cap   = 0;      /* allocated capacity for cache */
    static size_t   len   = 0;      /* number of terms currently stored (len >= 1) */

    /* initialize cache with a(1) if not yet done */
    if (cache == NULL) {
        cap = 1;
        len = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;         /* OOM: return 0 as fallback */
        cache[0] = 11;                      /* a(1) */
    }

    /* if requested term is already cached, return it */
    if (n <= len) return cache[n - 1];

    /* grow capacity until it can store term n */
    if ((size_t)n > cap) {
        size_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* OOM: return 0 as fallback */
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms from len+1 up to n */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i-1) */
        seq_t next;
        if (i & 1) {                       /* i is odd (>=3) */
            next = prev + 1;
        } else {                           /* i is even */
            next = (prev * prev - 1) / 2;
        }
        cache[i - 1] = next;
    }

    len = (size_t)n;
    return cache[n - 1];
}
