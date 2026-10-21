/*
 * A052695  Expansion of e.g.f. (2-5*x)/((1-x)*(1-4*x)).
 * https://oeis.org/A052695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4^n + 1) * n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence: a(0)=2, a(1)=5, a(n)=5*n*a(n-1) - 4*n*(n-1)*a(n-2) */

#include <stdlib.h>

/* Computes the n-th term of OEIS A052695. */
seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;   /* allocated size of cache          */
    static idx_t computed_max = 0;   /* highest index already computed   */

    /* Initialise cache for n=0,1 */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out of memory (unlikely in tests) */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        computed_max = 1;              /* indices 0 and 1 are known          */
    }

    /* Ensure cache large enough */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= computed_max) return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t i_seq        = (seq_t)i;
        seq_t i_minus1_seq = (seq_t)(i - 1);

        seq_t term1 = 5 * i_seq * cache[i - 1];
        seq_t term2 = 4 * i_seq * i_minus1_seq * cache[i - 2];
        cache[i] = term1 - term2;
    }

    computed_max = n;
    return cache[n];
}
