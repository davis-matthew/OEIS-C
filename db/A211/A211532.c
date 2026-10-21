/*
 * A211532  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          four, five, six or seven distinct values for every i,j,k<=n.
 * https://oeis.org/A211532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+2*a(n-2)-11*a(n-3)+a(n-4)+12*a(n-5)-2*a(n-6)-4*a(n-7)
 *            with a(1)=8, a(2)=22, a(3)=46, a(4)=100, a(5)=204, a(6)=422,
 *            a(7)=856
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of computed terms.  cache[0] is unused; cache[1] = a(1), ... */
static seq_t *cache = NULL;
static size_t cached = 0;  /* highest index currently computed */

/* Ensure that all terms up to and including n are stored in cache. */
static void ensure_cache(idx_t n)
{
    if (n < 1)
        return;

    /* First time: allocate at least up to index 7 and fill initial values. */
    if (cache == NULL) {
        size_t cap = (size_t) (n > 7 ? n : 7);
        cache = (seq_t*)calloc(cap + 1, sizeof(seq_t));
        if (cache == NULL) {
            /* Allocation failed – abort to avoid undefined behaviour. */
            abort();
        }
        cache[1] = 8;
        cache[2] = 22;
        cache[3] = 46;
        cache[4] = 100;
        cache[5] = 204;
        cache[6] = 422;
        cache[7] = 856;
        cached = 7;
    }

    /* Grow cache if necessary. */
    if ((size_t)n > cached) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;

        /* Compute new terms using the recurrence. */
        for (idx_t k = cached + 1; k <= n; ++k) {
            cache[k] = (seq_t)3 * cache[k-1]
                     + (seq_t)2 * cache[k-2]
                     - (seq_t)11 * cache[k-3]
                     + cache[k-4]
                     + (seq_t)12 * cache[k-5]
                     - (seq_t)2 * cache[k-6]
                     - (seq_t)4 * cache[k-7];
        }
        cached = (size_t)n;
    }
}

seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;   /* undefined for n <= 0, return 0 as a placeholder */

    ensure_cache(n);
    return cache[n];
}
