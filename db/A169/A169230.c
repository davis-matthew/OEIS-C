/*
 * A169230  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169230: Number of reduced words of length n in a
   Coxeter group with 25 generators and relations (S_i)^2 = (S_i S_j)^28 = I.
   Recurrence:
     a(0) = 1
     for 1 ≤ n ≤ 27:
        a(n) = 23 * (a(n-1)+a(n-2)+…+a(0)) + 2
     n = 28:
        a(28) = 23 * (a(27)+…+a(1)) – 276·a(0) + 1
     for n > 28:
        a(n) = 23 * (a(n-1)+a(n-2)+…+a(n-27)) – 276·a(n-28) */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t   cap   = 0;          /* current capacity of cache   */
    static idx_t    max_i = 0;          /* largest index already computed */

    /* ensure the cache can hold index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure – abort */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise a(0) once */
    if (max_i == 0) {
        cache[0] = (seq_t)1;
        max_i = 0;
    }

    if (n <= max_i)
        return cache[n];

    /* compute successive values up to n */
    for (idx_t i = max_i + 1; i <= n; ++i) {
        seq_t sum = 0;

        if (i <= 27) {                            /* 1 ≤ i ≤ 27 */
            for (idx_t k = 0; k < i; ++k)
                sum += cache[i - 1 - k];
            cache[i] = (seq_t)23 * sum + (seq_t)2;
        } else if (i == 28) {                    /* special n = 28 */
            for (idx_t k = 1; k <= 27; ++k)
                sum += cache[i - k];
            cache[i] = (seq_t)23 * sum
                       - (seq_t)276 * cache[0]
                       + (seq_t)1;
        } else {                                 /* i > 28 */
            for (idx_t k = 1; k <= 27; ++k)
                sum += cache[i - k];
            cache[i] = (seq_t)23 * sum
                       - (seq_t)276 * cache[i - 28];
        }
    }

    max_i = n;
    return cache[n];
}
