/*
 * A200572  Number of n X 1 0..2 arrays with no average of any element and its
 *          horizontal and vertical neighbors equal to one.
 * https://oeis.org/A200572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3) - 3*a(n-4) - 2*a(n-5);
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
// Initial values: a(1)=2, a(2)=6, a(3)=10, a(4)=22, a(5)=52
*/
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[1..] holds computed values          */
static idx_t  cache_cap = 0;         /* current allocated capacity              */
static idx_t  last_known = 0;        /* largest n that has been computed        */

/* Ensure there is room for indices up to <need>.  The capacity is only
   increased; reallocation is performed only when necessary. */
static void ensure_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    seq_t *new_cache = (seq_t *)malloc((size_t)need * sizeof(seq_t));
    if (!new_cache)                          /* abort on allocation failure */
        exit(1);
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = need;
}

/* Initialise the cache with the first five terms.  This is performed
   only once regardless of the order of calls. */
static void init_cache(void)
{
    if (last_known >= 5)            /* already initialised */
        return;
    ensure_cap(6);                  /* indices 0..5 will be available */
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)6;
    cache[3] = (seq_t)10;
    cache[4] = (seq_t)22;
    cache[5] = (seq_t)52;
    last_known = 5;
}

seq_t a(idx_t n)
{
    if (n == 0)                        /* defined only for n>=1 */
        return (seq_t)0;
    init_cache();

    if (n <= last_known)               /* already computed */
        return cache[n];

    /* Make sure there is space for the new entries. */
    ensure_cap(n + 5);

    /* Compute any remaining terms up to n using the recurrence. */
    for (idx_t i = last_known + 1; i <= n; ++i) {
        cache[i] = (seq_t)2   * cache[i-1]    /* 2*a(n-1)   */
                  +           cache[i-2]      /*   + a(n-2) */
                  +           cache[i-3]      /*   + a(n-3) */
                  - (seq_t)3 * cache[i-4]    /*   - 3*a(n-4) */
                  - (seq_t)2 * cache[i-5];   /*   - 2*a(n-5) */
    }
    last_known = n;
    return cache[n];
}
