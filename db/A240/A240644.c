/*
 * A240644  Number of nX3 0..1 arrays with no element equal to the same number
 *          of vertical neighbors as horizontal neighbors, with new values
 *          0..1 introduced in row major order.
 * https://oeis.org/A240644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -3*a(n-2)
 *            -2*a(n-3)+2*a(n-4)+2*a(n-5)-7*a(n-6)-8*a(n-7)+15*a(n-8)+4*a(n-9)-8*a(n-10)-4*a(n-11)+15*a(n-12)+8*a(n-13)-7*a(n-14)-2*a(n-15)+2*a(n-16)+2*a(n-17)-3*a(n-18)-4*a(n-19)-a(n-20);
 *            * Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=8, a(5)=23,
 *            a(6)=60, a(7)=149, a(8)=396, a(9)=105...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[20] = {
    (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)8, (seq_t)23,
    (seq_t)60, (seq_t)149, (seq_t)396, (seq_t)1050, (seq_t)2814,
    (seq_t)7571, (seq_t)20372, (seq_t)54879, (seq_t)147946,
    (seq_t)399002, (seq_t)1076312, (seq_t)2903613, (seq_t)7833652,
    (seq_t)21135287, (seq_t)57024772
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static const int coeffs[20] = {
    4, -3, -2,  2,  2,
   -7, -8, 15,  4, -8,
   -4, 15,  8, -7, -2,
    2,  2, -3, -4, -1
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (cache == NULL) {
        cache_len = 20;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < 20; ++i) {
            cache[i] = init_terms[i];
        }
    }
    if (n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        for (idx_t pos = cache_len + 1; pos <= n; ++pos) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (int i = 0; i < 20; ++i) {
                idx_t k = (idx_t)(i + 1);
                idx_t idx_cache = pos - k - 1;
                seq_t prod = cache[idx_cache] *
                             (seq_t)(coeffs[i] > 0 ? coeffs[i] : -coeffs[i]);
                if (coeffs[i] > 0) {
                    pos_sum += prod;
                } else {
                    neg_sum += prod;
                }
            }
            cache[pos - 1] = pos_sum - neg_sum;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
