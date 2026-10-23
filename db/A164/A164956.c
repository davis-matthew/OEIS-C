/*
 * A164956  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164956 – number of reduced words of length n in the Coxeter group
   with 22 generators S_i and relations S_i^2 = (S_i S_j)^8 = I.
   The sequence satisfies the linear recurrence

   a(0)=1, a(1)=22, a(2)=462, a(3)=9702, a(4)=203742,
   a(5)=4278582, a(6)=89850222, a(7)=1886854662

   a(8)   = 20*(a7+a6+a5+a4+a3+a2+a1) - 210*a0 + 1
   a(n)   = 20*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7))
            - 210*a(n-8)            for n ≥ 9
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;
    static size_t cache_cap = 0;

    /* given initial terms */
    const seq_t init[8] = { (seq_t)1, (seq_t)22, (seq_t)462,
                            (seq_t)9702, (seq_t)203742,
                            (seq_t)4278582, (seq_t)89850222,
                            (seq_t)1886854662 };

    /* first call: allocate and fill first eight values */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
        cache_len = 8;
    }

    /* grow cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        if (new_len > cache_cap) {
            cache_cap = new_len;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else if (i == 8) {
                /* special case for n=8 (RHS coefficient = 1) */
                seq_t sum = 0;
                for (idx_t k = 1; k <= 7; ++k)
                    sum += cache[i - k];
                cache[i] = (seq_t)20 * sum
                           - (seq_t)210 * cache[0]
                           + (seq_t)1;
            } else {
                seq_t sum = 0;
                for (idx_t k = 1; k <= 7; ++k)
                    sum += cache[i - k];
                cache[i] = (seq_t)20 * sum
                           - (seq_t)210 * cache[i - 8];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
