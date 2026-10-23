/*
 * A164350  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=49, a(2)=2352, a(3)=112896, a(4)=5419008,
 *            a(5)=260112384; For n=6: a(6)=47*(a(5)+a(4)+a(3)+a(2)+a(1)) -
 *            1128*a(0)+1; For n>6:
 *            a(n)=47*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) - 1128*a(n-6)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* Ensure cache can hold indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t newcap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (newcap < (size_t)n + 1) newcap <<= 1;
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = newptr;
        cache_cap = newcap;
    }

    /* Compute missing values lazily */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i < 6) {
            static const seq_t init_vals[6] = {
                (seq_t)1ULL,
                (seq_t)49ULL,
                (seq_t)2352ULL,
                (seq_t)112896ULL,
                (seq_t)5419008ULL,
                (seq_t)260112384ULL
            };
            val = init_vals[i];
        } else if (i == 6) {
            seq_t sum = cache[5];
            sum += cache[4];
            sum += cache[3];
            sum += cache[2];
            sum += cache[1];
            seq_t t = sum * (seq_t)47ULL;
            seq_t sub = cache[0] * (seq_t)1128ULL;
            val = t - sub + (seq_t)1ULL;   /* special +1 for n=6 */
        } else {  /* i > 6 */
            seq_t sum  = cache[i-1];
            sum += cache[i-2];
            sum += cache[i-3];
            sum += cache[i-4];
            sum += cache[i-5];
            seq_t t = sum * (seq_t)47ULL;
            seq_t sub = cache[i-6] * (seq_t)1128ULL;
            val = t - sub;
        }
        cache[i] = val;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
