/*
 * A164868  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164868:
   a(0)=1, a(1)=17, a(2)=272, a(3)=4352, a(4)=69632,
   a(5)=1114112, a(6)=17825792, a(7)=285212672.
   Recurrence (for n≥8):
   a(n) = 15*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7))
          -120*a(n-8) + (n==8 ? 1 : 0)   */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for 0≤n≤7 */
    static const seq_t init[8] = {
        (seq_t)1,
        (seq_t)17,
        (seq_t)272,
        (seq_t)4352,
        (seq_t)69632,
        (seq_t)1114112,
        (seq_t)17825792,
        (seq_t)285212672
    };

    if (n < (idx_t)8) {
        return init[n];
    }

    /* Lazy memoization: store computed terms in a static array */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (cache == NULL) {                          /* first call */
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i) {
            cache[i] = init[i];
        }
    }

    if ((size_t)n >= cache_size) {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {                        /* allocation failed */
            /* fallback: compute on demand without caching */
            cache = cache;
            cache_size = old_size;
        } else {
            cache = tmp;
            /* compute missing terms from old_size to n */
            for (size_t i = old_size; i <= n; ++i) {
                /* recurrence starts at n=8 */
                seq_t sum = cache[i-1];
                for (size_t j = 2; j <= 7; ++j) {
                    sum += cache[i-j];
                }
                seq_t term = sum * (seq_t)15;
                term -= cache[i-8] * (seq_t)120;
                if (i == 8) {
                    term += (seq_t)1;
                }
                cache[i] = term;
            }
            cache_size = new_size;
        }
    }

    return cache[n];
}
