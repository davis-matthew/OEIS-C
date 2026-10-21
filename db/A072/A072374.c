/*
 * A072374  a(1) = 1; a(n) = 1 + Sum_{i=1..n} Product_{j=i..2*i-1} (n-j).
 * https://oeis.org/A072374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 22, u64 n <= 38, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A072374: a(1)=1; for n≥1  
    Recurrence: 2*a(n) = 3*a(n-1) + (n-1)*a(n-2) – (n-1)*a(n-3)  
    with a(1)=1, a(2)=2, a(3)=3.  This gives the sequence 1,2,3,6,11,...  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms; cache[n-1] == a(n) for n≥1 */
    static seq_t *cache = NULL;   /* array of terms a(1)..a(max) */
    static idx_t max_computed = 0;    /* number of terms stored (max_computed ≥ 0) */

    if (n == 0)                /* undefined for n=0 – return 0 to avoid UB */
        return (seq_t)0;

    /* enlarge cache if it cannot hold a(n) */
    if (n > max_computed) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();     /* allocation failed */
        cache = tmp;

        /* compute missing terms from (max_computed+1) to n */
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            if (k == 1) {
                cache[0] = (seq_t)1;                /* a(1) */
            } else if (k == 2) {
                cache[1] = (seq_t)2;                /* a(2) */
            } else if (k == 3) {
                cache[2] = (seq_t)3;                /* a(3) */
            } else {
                /* 2*a(k) = 3*a(k-1) + (k-1)*(a(k-2)-a(k-3))   */
                seq_t a_prev1 = cache[k - 2];        /* a(k-1)   */
                seq_t a_prev2 = cache[k - 3];        /* a(k-2)   */
                seq_t a_prev3 = cache[k - 4];        /* a(k-3)   */
                seq_t diff = a_prev2 - a_prev3;      /* a(k-2) – a(k-3), non‑negative */
                seq_t term1 = (seq_t)3 * a_prev1;
                seq_t term2 = (seq_t)(k - 1) * diff; /* (k-1) as seq_t */
                cache[k - 1] = (term1 + term2) / (seq_t)2; /* exact division by 2 */
            }
        }
        max_computed = n;
    }

    return cache[n - 1];
}
