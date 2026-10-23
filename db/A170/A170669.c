/*
 * A170669  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1 Let r_n = 1 if n==0 or n==49, r_n = 2 if 1≤n≤48,
 *            else r_n = 0. Let S(n) = a_{n-1} + a_{n-2} + … +
 *            a_{max(0,n-48)}. Then a(n) = r_n + 34·S(n) + (595 if
 *            n≥49)·a_{n-49}
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A170669.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t   *cache = NULL;    /* stored values a[0..cache_size-1] */
    static idx_t   cache_size = 0;   /* number of terms stored */
    static idx_t   cache_cap   = 0;   /* capacity of cache array    */

    /* Allocate or grow cache if necessary.  */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* fatal error on allocation failure */
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n.  */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t r;
        if (k == 0)          r = 1;
        else if (k == 49)    r = 1;
        else if (k > 49)     r = 0;
        else                 r = 2;        /* 1 ≤ k ≤ 48 */

        seq_t sum = 0;
        idx_t limit = k < 48 ? k : 48;
        for (idx_t i = 1; i <= limit; ++i)
            sum += cache[k - i];

        seq_t term = r + (seq_t)34 * sum;
        if (k >= 49)
            term += (seq_t)595 * cache[k - 49];

        cache[k] = term;
    }
    cache_size = n + 1;
    return cache[n];
}
