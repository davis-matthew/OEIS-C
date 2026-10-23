/*
 * A143606  Expansion of x/(1+x-x^3-x^5-x^6-x^7-x^9+x^11+x^12).
 * https://oeis.org/A143606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A143606 (OEIS):  a(1)=1,
   for n>1   a(n) = –a(n‑1) + a(n‑3) + a(n‑5) + a(n‑6) + a(n‑7) + a(n‑9)
                     – a(n‑11) – a(n‑12)
   (indices <1 contribute 0).  The sequence starts: 1, ‑1, 1, 0, ‑1, 3, …
*/

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] is always 0 */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t max_computed = 0;   /* largest index already computed */

static void ensure_cache(idx_t n)
{
    if (n < cache_cap)        /* already have enough space */
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= n)
        new_cap <<= 1;        /* double until enough */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();             /* fatal error – out of memory */

    /* initialise whole array to 0 (cache[0] is 0 by default) */
    for (size_t i = 0; i < new_cap; ++i)
        new_cache[i] = 0;

    /* copy previous values */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined offset, but harmless */

    ensure_cache(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = -cache[i - 1];          /* –a(i‑1) */

        if (i >= 3)  val += cache[i - 3];   /* +a(i‑3) */
        if (i >= 5)  val += cache[i - 5];   /* +a(i‑5) */
        if (i >= 6)  val += cache[i - 6];   /* +a(i‑6) */
        if (i >= 7)  val += cache[i - 7];   /* +a(i‑7) */
        if (i >= 9)  val += cache[i - 9];   /* +a(i‑9) */
        if (i >= 11) val -= cache[i - 11];  /* –a(i‑11) */
        if (i >= 12) val -= cache[i - 12];  /* –a(i‑12) */

        if (i == 1)  val += (seq_t)1;       /* x term on RHS of G.F. */

        cache[i] = val;
    }

    if (n > max_computed)
        max_computed = n;

    return cache[n];
}
