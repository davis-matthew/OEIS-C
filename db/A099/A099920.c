/*
 * A099920  a(n) = (n+1)*F(n), F(n) = Fibonacci numbers A000045.
 * https://oeis.org/A099920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * F(n) where F(n) is the Fibonacci sequence with
 *            F(0)=0, F(1)=1. The function caches computed Fibonacci values to
 *            allow arbitrary order of calls in O(max n) total time
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc / realloc */

seq_t a(idx_t n)
{
    /* Static cache of Fibonacci numbers */
    static seq_t *fibs = NULL;      /* fibs[i] = F(i) */
    static idx_t cache_size = 0;    /* number of stored Fibonacci numbers */

    /* Initialise cache for n=0 and n=1 */
    if (cache_size == 0) {
        cache_size = 2;
        fibs = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        fibs[0] = (seq_t)0;
        fibs[1] = (seq_t)1;
    }

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(fibs, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;     /* out‑of‑memory: return 0 as safe default */
        fibs = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            fibs[i] = fibs[i-1] + fibs[i-2];
        }
        cache_size = new_size;
    }

    /* Compute (n+1) * F(n) */
    seq_t multiplier = ((seq_t)n) + 1;   /* cast index before arithmetic */
    return fibs[n] * multiplier;
}
