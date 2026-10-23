/*
 * A170635  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   from the rational generating function (t^48 + 2t^47 + … + 2t +
 *            1) / (1176t^48 – 48t^47 – … – 48t + 1) we obtain the linear
 *            recurrence a(0) = 1 for 1 ≤ n ≤ 47: a(n) = 2 + 48 ·
 *            (a(n‑1)+…+a(0)) a(48) = 1 + 48·(a(47)+…+a(1)) – 1176·a(0) for n
 *            ≥ 49: a(n) = 48·(a(n‑1)+…+a(n‑47)) – 1176·a(n‑48)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t* alloc_seq_array(size_t sz)
{
#ifdef __cplusplus
    return new seq_t[sz];
#else
    return (seq_t*)malloc(sz * sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t* p)
{
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t used = 0;
    static size_t cap   = 0;

    /* ensure the cache can hold index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 128;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t* newp = alloc_seq_array(new_cap);
        if (!newp) return (seq_t)0;            /* allocation failure */
        for (idx_t i = 0; i < used; ++i) newp[i] = cache[i];
        free_seq_array(cache);
        cache = newp;
        cap   = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = used; i <= n; ++i) {
        seq_t val = 0;
        if (i < 48) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) sum += cache[i - k];
            seq_t ncoef = (i == 0) ? (seq_t)1 : (seq_t)2;
            val = ncoef + (seq_t)48 * sum;
        } else if (i == 48) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 47; ++k) sum += cache[i - k];
            val = (seq_t)1 + (seq_t)48 * sum - (seq_t)1176 * cache[0];
        } else { /* i > 48 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 47; ++k) sum += cache[i - k];
            val = (seq_t)48 * sum - (seq_t)1176 * cache[i - 48];
        }
        cache[i] = val;
    }
    used = n + 1;
    return cache[n];
}
