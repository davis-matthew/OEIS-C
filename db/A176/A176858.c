/*
 * A176858  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=4, k=-1 and l=0.
 * https://oeis.org/A176858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, k=-1, l=0;
   a(n+1)=∑_{p=0}^{n} a(p)*a(n-p)-(n+1)  for n≥1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array holding computed terms              */
    static idx_t cache_size = 0;     /* current allocation size (>= last+1)        */
    static idx_t last_computed = 0;  /* highest index that is known               */

    /* initialise the cache with the first two terms */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t*)calloc((size_t)cache_size, sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed – return zero      */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        last_computed = 1;
    }

    if (n <= last_computed)               /* already known value */
        return cache[n];

    /* ensure the allocation is large enough for indices 0..n */
    while (cache_size <= n)
        cache_size <<= 1;                  /* grow exponentially */

    seq_t *tmp = (seq_t*)realloc(cache, (size_t)cache_size * sizeof(seq_t));
    if (tmp == NULL) return 0;              /* allocation failed – return zero      */
    cache = tmp;

    /* compute from the next unknown index up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* convolution a(i) = ∑_{p=0}^{i-1} a(p)*a(i-1-p) - i   (i = n+1) */
        for (idx_t p = 0; p < i; ++p) {
            sum += cache[p] * cache[i - 1 - p];
        }
        sum -= (seq_t)i;                     /* subtract (i) = (n+1)            */
        cache[i] = sum;
    }

    last_computed = n;                      /* all terms up to n are now known */
    return cache[n];
}
