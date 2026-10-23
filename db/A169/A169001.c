/*
 * A169001  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n>0: S(n) = a(n-1)+a(n-2)+ … + a(n-22) (terms with
 *            negative index are 0) b(n) = 1 if n==0 2 if 1≤n≤22 1 if n==23 0
 *            otherwise a(n) = b(n) + 34 * S(n) – 595 * a(n-23) (for n<23 the
 *            last term is 0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache of already computed values   */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size   */
    static idx_t computed   = 0;   /* number of terms computed */

    /* enlarge cache if needed   */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed; caller must handle */
        cache = tmp;
        cache_size = new_size;
    }

    /* first initialization   */
    if (computed == 0) {
#ifndef __cplusplus         /* trivial type – assignment is fine   */
        cache[0] = 1;            /* a(0) = 1   */
#else                      /* class type – need default construction   */
        new(&cache[0]) seq_t(1);
#endif
        computed = 1;
    }

    /* compute all missing terms up to n   */
    for (idx_t i = computed; i <= n; ++i) {
        /* S(i) = a(i-1)+...+a(i-22)   */
        seq_t sum_s = 0;
        for (idx_t k = 1; k <= 22 && i >= k; ++k) {
            sum_s += cache[i - k];
        }

        /* b(i)   */
        seq_t b = 0;
        if (i == 0)          b = 1;
        else if (i <= 22)    b = 2;
        else if (i == 23)    b = 1;

        /* recurrence   */
        seq_t val = b + 34 * sum_s;
        if (i >= 23)
            val -= 595 * cache[i - 23];

        /* store result   */
        if (i < computed) {            /* already constructed   */
            cache[i] = val;
        } else {                       /* newly allocated         */
#ifdef __cplusplus
            new(&cache[i]) seq_t(val);
#else
            cache[i] = val;
#endif
        }
    }

    computed = n + 1;
    return cache[n];
}
