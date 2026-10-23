/*
 * A168834  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=13⋅12^(n-1) for 1≤n≤19, and for n≥20
 *            a(n)=11·(a(n-1)+⋯+a(n-19))−66·a(n-20). (Derived from the
 *            rational growth series of the Coxeter group.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* array of already computed values   */
static size_t cache_cap = 0;         /* current capacity of the array      */
static idx_t cache_max = (idx_t)-1;  /* largest n that has been computed   */

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) exit(1);              /* memory allocation failed */
        if (cache) {
            memcpy(tmp, cache, (size_t)(cache_max + 1) * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == (idx_t)0) {
            cache[i] = (seq_t)1;
        } else if (i <= 19) {
            /* a(i) = 13·12^(i-1) */
            seq_t pow = (seq_t)1;
            for (idx_t k = 1; k < i; ++k) pow *= (seq_t)12;
            cache[i] = pow * (seq_t)13;
        } else {
            /* recurrence for i >= 20 */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 19; ++j) sum += cache[i - j];
            cache[i] = sum * (seq_t)11 - cache[i - 20] * (seq_t)66;
        }
    }
    cache_max = n;
    return cache[n];
}
