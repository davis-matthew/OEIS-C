/*
 * A008749  Expansion of (1+x^6)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5; * for n≥6:
 *            a(n)=a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6)+(n==6?1:0)
 * Range:     u8 n <= 39, u16 n <= 627, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Expansion of (1+x^6)/((1-x)*(1-x^2)*(1-x^3)). */

#include <stdlib.h>  /* for realloc, size_t */
#include <assert.h>  /* for assert */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of computed terms */
    static idx_t cache_size = 0;    /* number of computed terms (highest index+1) */

    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);          /* new capacity needed */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        assert(tmp != NULL);                       /* allocation failure – abort */
        cache = tmp;

        idx_t start = cache_size;                  /* compute missing terms only */
        for (idx_t i = start; i <= n; ++i) {
            /* initial values for 0…5 */
            if (i == 0)         cache[i] = 1;
            else if (i == 1)    cache[i] = 1;
            else if (i == 2)    cache[i] = 2;
            else if (i == 3)    cache[i] = 3;
            else if (i == 4)    cache[i] = 4;
            else if (i == 5)    cache[i] = 5;
            else {
                /* recurrence: a(n)=a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6)+(n==6?1:0) */
                seq_t sum = cache[i-1] + cache[i-2] + cache[i-6];
                if (i == 6) sum += 1;
                cache[i] = sum - cache[i-4] - cache[i-5];
            }
        }
        cache_size = (idx_t)new_size;
    }
    return cache[n];
}
