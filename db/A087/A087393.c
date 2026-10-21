/*
 * A087393  a(1) = 1, a(n) = smallest number beginning with a(n-1) == 0 (mod
 *          n!).
 * https://oeis.org/A087393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation of A087393:
   a(1) = 1
   a(n) for n>1 : smallest integer starting with a(n-1) that is
   divisible by n!    (search for the minimal possible k)
*/
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of computed terms, 1‑based */
    static size_t  cache_cap = 0;   /* current allocated size */
    static idx_t   last = 0;        /* highest index already computed */
    static seq_t   fac   = (seq_t)1;/* factorial of 'last' */

    if (n == 0) return (seq_t)0; /* offset starts at 1, unused result */

    /* Allocate or reallocate the cache to fit indices up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* realloc may return NULL on failure; this is acceptable */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise a(1) if this is the first call */
    if (last == 0) {
        cache[1] = (seq_t)1;
        fac = (seq_t)1;
        last = 1;
    }

    /* Return cached value if already computed */
    if (n <= last) return cache[n];

    /* Compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Update factorial: fac = i! */
        fac = fac * (seq_t)i;

        seq_t prev = cache[i - 1];          /* a(i‑1) */
        seq_t lower = prev;                 /* current number prefix with 10^k */
        seq_t r = prev % fac;               /* remainder of prev*10^k modulo fac */
        seq_t pow10 = (seq_t)1;             /* 10^k, starting with k=0 */

        /* Search for the smallest k such that (fac - r) % fac < 10^k */
        while (1) {
            seq_t delta = (fac - r) % fac;  /* required addition to reach multiple of fac */
            if (delta < pow10) {
                cache[i] = lower + delta;   /* found the minimal number */
                break;
            }
            /* Increase k: multiply by 10 */
            r = (r * (seq_t)10) % fac;
            lower = lower * (seq_t)10;
            pow10 = pow10 * (seq_t)10;
        }
    }

    last = n;
    return cache[n];
}
