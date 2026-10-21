/*
 * A074046  a(n)=a(n-1)*a(n-2)*a(n-3)*(1/a(n-1)+1/a(n-2)+1/a(n-3)) starting
 *          with a(1)=a(2)=1 and a(3)=2.
 * https://oeis.org/A074046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*a(n-2)+a(n-1)*a(n-3)+a(n-2)*a(n-3) a(1) = 1, a(2)
 *            = 1, a(3) = 2 (OEIS A074046)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array 1‑based, cache[0] unused      */
static idx_t  last  = 0;      /* largest index already filled      */
static size_t cap   = 0;      /* allocated capacity of cache array  */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* safety for out of range indices */

    size_t new_cap = (size_t)n + 1;
    if (new_cap < 4) new_cap = 4;                         /* need space for a(1..3) */

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                                   /* allocation failed  */
    cache = tmp;
    cap   = new_cap;

    /* initialize the first three terms if not already done */
    if (last < 3) {
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        last = 3;
    }

    /* compute any missing terms up to n */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            cache[i] = a1 * a2 + a1 * a3 + a2 * a3;
        }
        last = n;
    }

    return cache[n];
}
