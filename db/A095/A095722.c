/*
 * A095722  E.g.f.: exp(x)/(1-x)^8.
 * https://oeis.org/A095722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A095722  
    Formula (closed form): a(n) = Σ_{k=0..n} binomial(n,k) * (k+7)! / 7!  
    Recurrence derived from the above formula:  
            a(0) = 1, a(1) = 9  
            a(n) = (n+8) * a(n-1) - (n-1) * a(n-2)   for n ≥ 2  
    This 2‑step recurrence with polynomial coefficients generates the
    sequence efficiently in O(n) time and works for arbitrary‑precision
    seq_t or unsigned integer types. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation – works for calls in any order */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* initialise cache with a(0)=1, a(1)=9 */
    if (cache == NULL) {
        cache_cap = 4;                  /* start with some spare capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* on allocation failure return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_size = 2;
    }

    /* extend cache if needed */
    if (n >= cache_size) {
        /* grow capacity if required */
        while (cache_size <= n) {
            if (cache_cap == 0) cache_cap = 4;
            if (cache_size >= cache_cap) {
                idx_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) return 0;   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* compute next term using the recurrence */
            idx_t m = cache_size;

            seq_t term1 = cache[m-1] * (seq_t)(m + 8);   /* (m+8)*a(m-1) */
            seq_t term2 = cache[m-2] * (seq_t)(m - 1);   /* (m-1)*a(m-2) */

            cache[m] = term1 - term2;   /* a(m) = term1 - term2 */

            cache_size++;
        }
    }

    return cache[n];
}
