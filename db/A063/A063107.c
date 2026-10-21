/*
 * A063107  Dimension of the space of weight 2n cusp forms for Gamma_0( 39 ).
 * https://oeis.org/A063107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) (n > 5) initial values: a(1)=3,
 *            a(2)=12, a(3)=22, a(4)=30, a(5)=40
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  For n<1 the value is undefined,
       but we return 0 for safety. */
    if (n < 1) return (seq_t)0;

    /* Static cache for already computed terms. */
    static seq_t *cache    = NULL;   /* array of values, 1‑indexed */
    static size_t cap      = 0;      /* allocated capacity (>= n+1) */
    static size_t computed = 0;      /* highest n computed so far */

    /* Allocate or enlarge the cache if necessary.  Do this on the first
       call or when n exceeds the current capacity. */
    if (cache == NULL || (size_t)n >= cap)
    {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 6;          /* at least 6 for the first 5 values */
        /* double until we can hold index n */
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;       /* allocation failure → 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the first five terms the first time the cache is used. */
    if (computed < 5)
    {
        cache[0] = (seq_t)0;      /* unused slot for 0‑based array */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)22;
        cache[4] = (seq_t)30;
        cache[5] = (seq_t)40;
        computed = 5;
    }

    /* Compute missing terms up to n (if any). */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        /* a(i) = a(i-1) + a(i-3) - a(i-4)  (valid for i>5) */
        cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 4];
    }
    computed = (size_t)n;

    return cache[(size_t)n];
}
