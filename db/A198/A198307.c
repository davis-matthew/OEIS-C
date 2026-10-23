/*
 * A198307  Moore lower bound on the order of a (7,g)-cage.
 * https://oeis.org/A198307
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 8 , a(4)=14 , a(5)=50 for n > 5: a(n) = a(n-1) + 6*a(n-2)
 *            - 6*a(n-3) Equivalently (for n≥3): if n is even : a(n) =
 *            2*(6^(n/2)-1)/5 if n is odd : a(n) = (7*6^((n-1)/2)-2)/5 this
 *            implementation uses the linear recurrence with memoisation
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy static memoisation of values up to the largest n seen */
    static seq_t *cache  = NULL;
    static idx_t  cap    = 0;     /* current capacity of cache (number of entries) */
    static idx_t  upto   = 0;     /* maximum n currently stored in cache */

    if (n < 3)
        return (seq_t)0;          /* undefined for n < 3 – offset starts at 3 */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 32;                 /* initial capacity – can grow later */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;       /* allocation failure – fallback to 0 */

        /* set trivial values for indices 0..2 (unused) */
        for (idx_t i = 0; i < 3; ++i)
            cache[i] = (seq_t)0;

        /* base values of the sequence */
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)14;
        cache[5] = (seq_t)50;
        upto    = 5;
    }

    /* enlarge buffer if necessary */
    if ((idx_t)upto < n) {
        size_t needed = (size_t)n + 1;           /* storage for indices 0..n */
        if (needed > cap) {
            size_t new_cap = cap * 2;
            if (new_cap < needed)
                new_cap = needed;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)          /* allocation failed – keep old cache */
                return (seq_t)0;
            cache = tmp;
            cap   = new_cap;
        }

        /* fill in missing values using the recurrence */
        for (idx_t i = upto + 1; i <= n; ++i) {
            if (i <= 5)         /* base terms already set */
                continue;
            cache[i] = cache[i-1] + (seq_t)6 * cache[i-2] - (seq_t)6 * cache[i-3];
        }
        upto = n;
    }

    return cache[n];
}
