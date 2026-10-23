/*
 * A168872  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - … - a(n-20) Initial
 *            values: a(0)=1; for 1≤n≤20, a(n)=3*2^(n-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size */
    static idx_t len   = 0;   /* number of terms stored */

    /* Grow the cache if necessary */
    if (n >= cap) {
        idx_t newcap = n + 1;
        if (newcap < 64)
            newcap = 64;
        while (newcap <= n)
            newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Initialise first 21 terms once */
    if (len == 0) {
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= 20; ++i)
            cache[i] = (seq_t)3 * (((seq_t)1) << (i - 1));
        len = 21;
    }

    /* Return if already computed */
    if (n < len)
        return cache[n];

    /* Compute remaining terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        /* For i ≤ 20 the value is already set; skip */
        if (i <= 20)
            continue;

        seq_t pos = (seq_t)0;   /* sum of terms with +2 coefficient */
        seq_t neg = (seq_t)0;   /* sum of terms with –1 coefficient */

        for (int k = 1; k <= 20; ++k) {
            idx_t idx = i - (idx_t)k;
            if (k & 1)   /* odd k → coefficient +2 */
                pos += cache[idx] << 1;   /* multiply by 2 */
            else           /* even k → coefficient –1 */
                neg += cache[idx];
        }
        cache[i] = pos - neg;   /* pos ≥ neg for all n */
    }

    len = n + 1;
    return cache[n];
}
