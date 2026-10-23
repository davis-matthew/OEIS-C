/*
 * A124024  G.f.: 1/(1+2*x-9*x^2-10*x^3+5*x^4).
 * https://oeis.org/A124024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) + 2*a(n-1) – 9*a(n-2) – 10*a(n-3) + 5*a(n-4) = 0
   for n ≥ 4, with a(0)=1, a(1)=-2, a(2)=13, a(3)=-34. */
#include <stdlib.h>

static seq_t *cache    = NULL;   /* memoised values          */
static idx_t  cache_sz = 0;      /* number of stored values  */
static idx_t  cache_cap= 0;      /* allocated capacity       */

/* Ensure capacity for at least `new_cap` elements.  */
static void enlarge(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_bytes = (size_t)new_cap * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, new_bytes);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* If already computed, return cached value. */
    if (n < cache_sz)
        return cache[n];

    /* We will need at least n+1 elements; also allocate the four
       fundamental seeds when they haven't been created yet. */
    if (cache_cap < n + 1)
        enlarge(n + 1);
    if (cache_cap < 4)  /* provide storage for the first four terms */
        enlarge(4);

    /* Initialise the first four terms if they haven't been set. */
    if (cache_sz == 0) { cache[0] = 1;   cache_sz = 1; }
    if (cache_sz == 1) { cache[1] = -2;  cache_sz = 2; }
    if (cache_sz == 2) { cache[2] = 13;  cache_sz = 3; }
    if (cache_sz == 3) { cache[3] = -34; cache_sz = 4; }

    /* Generate remaining terms up to n.  The recurrence is
       a(n) = -2*a(n-1) + 9*a(n-2) + 10*a(n-3) - 5*a(n-4). */
    for (idx_t i = cache_sz; i <= n; ++i) {
        cache[i] = -2 * cache[i-1] + 9 * cache[i-2]
                 + 10 * cache[i-3] - 5 * cache[i-4];
    }
    cache_sz = n + 1;
    return cache[n];
}
