/*
 * A101504  a(n+1)=a(n)/x+x, with x = Max(a(k): 0<=k<n and a(k)|a(n)),
 *          a(0)=a(1)=1.
 * https://oeis.org/A101504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A101504
    a(0) = a(1) = 1
    For n ≥ 1   a(n+1) = a(n)/x + x
    where     x = max{ a(k) : 0 ≤ k < n  and  a(k) divides a(n) }
*/
#include <stdlib.h>

static seq_t *cache = NULL;           /* array of computed terms        */
static idx_t  cache_size = 0;         /* number of elements in cache    */
static idx_t  cache_capacity = 0;     /* current allocated capacity     */

/* ensure that cache holds at least the term a(n) */
static void
ensure_cache(idx_t n)
{
    /* initialize cache with the first two values */
    if (cache == NULL) {
        cache_capacity = 2;
        cache_size    = 2;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) exit(1);     /* allocation failure */
        cache[0] = 1;
        cache[1] = 1;
    }

    while (cache_size <= n) {
        seq_t cur = cache[cache_size - 1];   /* a(n)   */
        seq_t x   = 1;                      /* at least 1 divides cur */

        /* find the maximum earlier divisor of cur */
        for (idx_t j = 0; j < cache_size - 1; ++j) {
            if (cur % cache[j] == 0 && cache[j] > x) {
                x = cache[j];
            }
        }

        seq_t nxt = cur / x + x;            /* a(n+1) */

        /* grow cache if necessary */
        if (cache_size == cache_capacity) {
            idx_t new_cap = cache_capacity * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(1);
            cache = tmp;
            cache_capacity = new_cap;
        }

        cache[cache_size++] = nxt;
    }
}

/* required public function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
