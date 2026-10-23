/*
 * A190917  Number of permutations of n copies of 1..3 introduced in order
 *          1..3 with no element equal to another within a distance of 1.
 * https://oeis.org/A190917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A190917: Number of permutations of n copies of 1..3 introduced in order
   with no element equal to another within a distance of 1.  
   Recurrence (for n ≥ 2):
     n*(n+1)*a(n) = (n+1)*(7*n-4)*a(n-1) + 8*(n-2)^2*a(n-2)
   Initial values:
     a(0) = 1, a(1) = 1
   This implementation memoises results and works for any seq_t:
   - if seq_t has ≤ 64 bits, 128‑bit intermediates are used to avoid overflow
   - for larger seq_t (e.g. arbitrary‑precision), all arithmetic is performed
     directly on seq_t objects.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* static memoisation buffer */
    static seq_t *cache = NULL;
    static idx_t  cached_up_to = 0;

    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));   /* indices 0 and 1 */
        if (cache == NULL) return 0;                  /* allocation error */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cached_up_to = 1;
    }

    /* grow cache if needed */
    if (n > cached_up_to) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;                    /* allocation error */
        cache = tmp;

        for (idx_t k = cached_up_to + 1; k <= n; ++k) {
            if (sizeof(seq_t) <= 8) { /* 64‑bit or smaller: use 128‑bit intermediates */
                unsigned __int128 term1 = (unsigned __int128)(k + 1)
                                           * (unsigned __int128)(7 * k - 4)
                                           * (unsigned __int128)cache[k - 1];
                unsigned __int128 term2 = (unsigned __int128)8
                                           * (unsigned __int128)(k - 2)
                                           * (unsigned __int128)(k - 2)
                                           * (unsigned __int128)cache[k - 2];
                unsigned __int128 numerator   = term1 + term2;
                unsigned __int128 denominator = (unsigned __int128)k
                                                * (unsigned __int128)(k + 1);
                cache[k] = (seq_t)(numerator / denominator);
            } else { /* seq_t is larger (e.g. arbitrary‑precision) */
                seq_t term1 = (seq_t)(k + 1) * (seq_t)(7 * k - 4) * cache[k - 1];
                seq_t term2 = (seq_t)8 * (seq_t)(k - 2) * (seq_t)(k - 2)
                              * cache[k - 2];
                seq_t numerator   = term1 + term2;
                seq_t denominator = (seq_t)k * (seq_t)(k + 1);
                cache[k] = numerator / denominator; /* exact division */
            }
        }
        cached_up_to = n;
    }

    return cache[n];
}
