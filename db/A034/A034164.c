/*
 * A034164  Related to triple factorial numbers 2*A034000(n+1).
 * https://oeis.org/A034164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034164: a(n) = 3^n * (3*n+2)!!! / (n+2)! */
/* Recurrence: a(0) = 1; for n ≥ 1,
   a(n) = a(n-1) * 3 * (3*n + 2) / (n + 2) */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;   /* allocated capacity of the cache   */
    static idx_t  last_comp = 0;   /* largest n that has been computed */

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!cache) return 0;               /* out‑of‑memory – return 0 as failure */
        cache_cap = n + 1;
        cache[0] = (seq_t)1;                /* a(0) = 1 */
    } else if ((size_t)(n + 1) > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = n + 1;
    }

    /* Compute missing values up to n */
    if (n > last_comp) {
        for (idx_t k = last_comp + 1; k <= n; ++k) {
            seq_t prev  = cache[k - 1];
            /* numerator = 3 * (3*k + 2) */
            seq_t num   = (seq_t)3 * ((seq_t)3 * k + (seq_t)2);
            /* denominator = k + 2 */
            seq_t denom = (seq_t)(k + 2);

            /* Compute next = prev * num / denom  avoiding intermediate overflow */
            seq_t div  = prev / denom;
            seq_t rem  = prev % denom;
            seq_t next = div * num + (rem * num) / denom;

            cache[k] = next;
        }
        last_comp = n;
    }

    return cache[n];
}
