/*
 * A164601  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -55*a(n-7) + 10*Sum_{k=1..6} a(n-k) (Offset 0: a(0)=1,
 *            a(1)=12, a(2)=132, a(3)=1452, a(4)=15972, a(5)=175692,
 *            a(6)=1932612, a(7)=21258666)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* Initial values up to n = 7 */
    const seq_t init[8] = {
        (seq_t)1,          /* a(0) */
        (seq_t)12,         /* a(1) */
        (seq_t)132,        /* a(2) */
        (seq_t)1452,       /* a(3) */
        (seq_t)15972,      /* a(4) */
        (seq_t)175692,     /* a(5) */
        (seq_t)1932612,    /* a(6) */
        (seq_t)21258666    /* a(7) */
    };

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failed */
        cache = new_cache;

        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                seq_t sum = 0;
                for (idx_t k = 1; k <= 6; ++k)
                    sum += cache[i - k];
                cache[i] = 10 * sum;
                cache[i] -= 55 * cache[i - 7];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
