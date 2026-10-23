/*
 * A172613  a(n) = (9n)!/(9!^n).
 * https://oeis.org/A172613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)! / (9!^n) (= ∏_{k=1..n} C(9k,9), a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   binom_9i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom_9i.h"

/* Memoised computation of the sequence.  cache[n] holds a(n). */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of values */
    static size_t  cache_cap = 0;     /* allocated size          */
    static size_t  computed  = 0;     /* number of values made   */

    if (n >= computed) {
        /* ensure enough capacity */
        if (cache_cap <= n) {
            size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
            while (new_cap <= n)
                new_cap *= 2;

            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* fill in missing values */
        for (size_t i = computed; i <= n; ++i) {
            if (i == 0)
                cache[0] = (seq_t)1;                   /* a(0) = 1 */
            else
                cache[i] = cache[i - 1] * binom_9i(i);
        }
        computed = n + 1;
    }

    return cache[n];
}
