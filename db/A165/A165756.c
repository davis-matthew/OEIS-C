/*
 * A165756  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*∑_{i=1}^{9} a(n-i)
 *            - 3*a(n-10) for n ≥ 11 a(10) = 2*∑_{i=1}^{9} a(10-i) - 3*a(0) +
 *            1 initial values: a(0)=1, a(1)=4, a(2)=12, a(3)=36, a(4)=108,
 *            a(5)=324, a(6)=972, a(7)=2916, a(8)=8748, a(9)=26244
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* dynamic cache of computed terms */
static idx_t cache_len = 0;          /* number of terms stored (cache_len = max_index+1) */
static idx_t cache_cap = 0;          /* current capacity of cache */

/* first ten terms */
static const seq_t init10[10] = {
    (seq_t)1,
    (seq_t)4,
    (seq_t)12,
    (seq_t)36,
    (seq_t)108,
    (seq_t)324,
    (seq_t)972,
    (seq_t)2916,
    (seq_t)8748,
    (seq_t)26244
};

seq_t a(idx_t n)
{
    /* grow cache to hold up to n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t value;
        if (k < 10) {
            value = init10[k];
        } else if (k == 10) {
            /* compute sum of previous 9 terms */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 9; ++j)
                sum += cache[k - j];
            value = (sum * (seq_t)2) - (cache[k - 10] * (seq_t)3) + (seq_t)1;
        } else { /* k >= 11 */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 9; ++j)
                sum += cache[k - j];
            value = (sum * (seq_t)2) - (cache[k - 10] * (seq_t)3);
        }
        cache[k] = value;
    }
    cache_len = n + 1;
    return cache[n];
}
