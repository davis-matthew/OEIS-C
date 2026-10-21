/*
 * A213578  Antidiagonal sums of the convolution array A213576.
 * https://oeis.org/A213578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6)
 *            Offset: 1
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 80, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------
   Memoized computation of A213578 using the linear recurrence
   ------------------------------------------------------------------ */

static seq_t *cache   = NULL;   /* 1‑based cache, index 0 unused */
static idx_t  upto    = 0;      /* largest n that is cached */
static size_t cap     = 0;      /* current capacity of cache */

/* Return a(n) for any n >= 1, computing missing terms with the
   recurrence.  The function is callable in any order and keeps a
   persistent cache between calls. */
seq_t a(idx_t n)
{
    /* Allocate enough room for indices up to n (at least up to 6). */
    size_t need = (n + 1 > 7) ? (n + 1) : 7;   /* include index 0 */

    if (cache == NULL) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure */
        cap = need;
        /* initialise first six terms */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)34;
        cache[5] = (seq_t)80;
        cache[6] = (seq_t)174;
        upto = 6;
    } else if (need > cap) {                  /* grow array if required */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        cap = need;
    }

    if (n <= upto)                  /* already computed */
        return cache[n];

    /* Compute all missing terms up to n */
    for (idx_t i = upto + 1; i <= n; ++i) {
        /* i-6 is valid for i>=7, for i=7..6 we skip because i>upto>=6 */
        seq_t v1 = cache[i-1] * 4;          /* 4*a(i-1) */
        seq_t v2 = cache[i-2] * 4;          /* 4*a(i-2) */
        seq_t v3 = cache[i-3] * 2;          /* 2*a(i-3) */
        seq_t v4 = cache[i-4] * 4;          /* 4*a(i-4) */
        seq_t v6 = cache[i-6];              /* a(i-6) */

        seq_t res = v1 - v2 - v3 + v4 - v6;
        cache[i] = res;
    }

    upto = n;
    return cache[n];
}
