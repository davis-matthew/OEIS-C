/*
 * A162827  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=27, a(2)=702; for n>=3:
 *            a(n)=25*a(n-1)+25*a(n-2)-325*a(n-3)+[n==3]; (the extra +1 occurs
 *            only at n = 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t last_computed = 0;

    if (cache == NULL) {
        /* initial allocation for first call */
        idx_t init_len = n + 1;
        if (init_len < 16) init_len = 16;
        cache = (seq_t*)malloc(init_len * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_len = init_len;
        cache[0] = 1;          /* a(0) */
        last_computed = 0;
    }

    if (n > last_computed) {
        /* grow buffer if necessary */
        if (cache_len <= n) {
            idx_t new_len = cache_len * 2;
            while (new_len <= n) new_len *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            cache_len = new_len;
        }

        /* compute missing terms */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = 27;                                   /* a(1) */
            else if (i == 2)
                cache[i] = 702;                                  /* a(2) */
            else if (i == 3)
                cache[i] = 25 * cache[2] + 25 * cache[1]
                           - 325 * cache[0] + 1;                /* a(3) */
            else
                cache[i] = 25 * cache[i-1] + 25 * cache[i-2]
                           - 325 * cache[i-3];                 /* general step */
        }
        last_computed = n;
    }

    return cache[n];
}
