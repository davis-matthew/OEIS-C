/*
 * A165341  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + Σ_{k=1}^{8} 17·a(n−k) – 153·a(n−9) * where p_n = 1
 *            if n = 0 or n = 9 * = 2 if 1 ≤ n ≤ 8 * = 0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms   */
static idx_t  cache_len  = 0; /* number of computed terms  */
static idx_t  cache_cap  = 0; /* allocated capacity       */

static void ensure_cache_cap(idx_t n)   /* ensure capacity for index n          */
{
    /* we need space for indices 0 … n inclusive */
    idx_t need = n + 1;
    if (need <= cache_cap)
        return;
    idx_t newcap = cache_cap ? cache_cap * 2 : 16;
    if (newcap < need)
        newcap = need;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
    if (!tmp)
        abort();                         /* allocation failure */
    cache = tmp;
    if (newcap > cache_cap) {
        for (idx_t i = cache_cap; i < newcap; ++i)
            cache[i] = (seq_t)0;
    }
    cache_cap = newcap;
}

/* Return the n-th term of the sequence A165341.  */
seq_t a(idx_t n)
{
    /* make sure the cache can hold indices up to n */
    ensure_cache_cap(n);

    /* compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = 0;
        if (i <= 9) {                /* contribution from the numerator */
            term = (seq_t)((i == 0 || i == 9) ? 1 : 2);
        }

        seq_t sum = 0;
        /* Σ_{k=1}^{8} 17·a(i−k)  with a(j)=0 for j<0 */
        for (idx_t k = 1; k <= 8; ++k) {
            if (i >= k)
                sum += (seq_t)17 * cache[i - k];
        }

        /* −153·a(i−9)  with a(j)=0 for j<0 */
        if (i >= 9)
            sum -= (seq_t)153 * cache[i - 9];

        cache[i] = sum + term;
    }

    cache_len = n + 1;
    return cache[n];
}
