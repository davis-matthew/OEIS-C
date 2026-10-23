/*
 * A160333  Number of pairs of rabbits in month n in the dying rabbits
 *          problem, if they become mature after 4 months and give birth to
 *          exactly 7 pairs, one per month.
 * https://oeis.org/A160333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=a(3)=1 a(n)=a(n-1)+a(n-4) for 4≤n≤9
 *            a(n)=a(n-1)-a(n-2)+a(n-3)+a(n-5)+a(n-7)+a(n-9) for n≥10
 * Range:     u8 n <= 19, u16 n <= 38, u32 n <= 74, u64 n <= 147,
 *            u128 n <= 294, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;      /* capacity allocated in cache */
    static idx_t   last   = 0;      /* highest index already computed */

    /* initialise the cache on the first call */
    if (cache == NULL) {
        cap = 1;                    /* at least one element (a(0))         */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure – return 0 */
        cache[0] = (seq_t)1;       /* a(0)                                  */
        last = 0;                   /* we have computed a(0)                 */
    }

    /* enlarge cache if the requested index is outside current capacity */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure – return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    while (last < n) {
        idx_t i = last + 1;
        seq_t val;

        if (i <= 3) {
            val = (seq_t)1;
        } else if (i <= 9) {
            val = cache[i-1] + cache[i-4];
        } else {
            /* recurrence for n ≥ 10 */
            val = cache[i-1] - cache[i-2] + cache[i-3]
                  + cache[i-5] + cache[i-7] + cache[i-9];
        }

        cache[i] = val;
        last = i;
    }

    return cache[n];
}
