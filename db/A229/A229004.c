/*
 * A229004  Indices of Bell numbers divisible by 3.
 * https://oeis.org/A229004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5); a(1)=4, a(2)=8, a(3)=9,
 *            a(4)=11, a(5)=17
 * Range:     u8 n <= 78, u16 n <= 9446, u32 n <= 9446, u64 n <= 9446,
 *            u128 n <= 9446, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based cache: cache[i] holds a(i) for i>=1 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* capacity (number of usable slots) */
    static idx_t last_computed = 0;   /* largest n whose value is stored */
    static int  initialized   = 0;    /* flag for first call */

    /*--- initialization ---------------------------------------------------*/
    if (!initialized) {
        cache_cap = 6;                               /* indices 0..5 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)11;
        cache[5] = (seq_t)17;
        last_computed = 5;
        initialized   = 1;
    }

    /*--- grow cache if required ------------------------------------------*/
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /*--- if already computed, return immediately -------------------------*/
    if (n <= last_computed)
        return cache[n];

    /*--- compute missing values ------------------------------------------*/
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 5) {
            /* initial values already written in init block */
            continue;
        }
        cache[i] = cache[i - 1] + cache[i - 4] - cache[i - 5];
    }
    last_computed = n;

    return cache[n];
}
